#include "RedisServer.h"

#include <sys/epoll.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#include <fcntl.h>
#include <unistd.h>

#include <cerrno>
#include <cstring>

#include <iostream>
#include <memory>
#include <unordered_map>
RedisServer::RedisServer(int port)
    :
    port(port),
    server_fd(-1),
    epoll_fd(-1),
    executor(datastore),
    processor(
        commandParser,
        respParser,
        respEncoder,
        executor
    )
{
}

RedisServer::~RedisServer()
{
    if(server_fd != -1)
        close(server_fd);

    if(epoll_fd != -1)
        close(epoll_fd);
}

void RedisServer::start()
{
    initializeSocket();

    initializeEpoll();

    eventLoop();
}

void RedisServer::initializeSocket()
{
    server_fd = socket(
        AF_INET,
        SOCK_STREAM,
        0
    );

    if(server_fd == -1)
    {
        throw std::runtime_error(
            std::string("Failed to create socket")+
            strerror(errno)
        );
    }

    int opt = 1;

    if(
        setsockopt(
            server_fd,
            SOL_SOCKET,
            SO_REUSEADDR,
            &opt,
            sizeof(opt)
        ) == -1
    )
    {
        close(server_fd);
        throw std::runtime_error(
            std::string("setsockopt() failed")+
            strerror(errno)
        );
    }

    sockaddr_in address{};

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if(
        bind(
            server_fd,
            reinterpret_cast<sockaddr*>(&address),
            sizeof(address)
        ) == -1
    )
    {
        close(server_fd);
        throw std::runtime_error(
            std::string("bind() failed")+
            strerror(errno)
        );
    }

    if(
        listen(
            server_fd,
            SOMAXCONN
        ) == -1
    )
    {
        close(server_fd);
        throw std::runtime_error(
            std::string("listen() failed")+
            strerror(errno)
        );
    }

    setNonBlocking(server_fd);
}

void RedisServer::initializeEpoll()
{
    epoll_fd = epoll_create1(0);

    if(epoll_fd == -1)
    {
        throw std::runtime_error(
            std::string("epoll_create1 failed: ") +
            strerror(errno)
        );
    }

    epoll_event event{};

    event.events =
        EPOLLIN |
        EPOLLET;

    event.data.fd = server_fd;

    if(
        epoll_ctl(
            epoll_fd,
            EPOLL_CTL_ADD,
            server_fd,
            &event
        ) == -1
    )
    {
        close(epoll_fd);

        throw std::runtime_error(
            std::string("epoll_ctl failed: ") +
            strerror(errno)
        );
    }
}

void RedisServer::acceptConnections()
{
    while(true)
    {
        sockaddr_in clientAddr{};
        socklen_t clientLen = sizeof(clientAddr);

        int client_fd =
            accept(
                server_fd,
                reinterpret_cast<sockaddr*>(&clientAddr),
                &clientLen
            );

        if(client_fd == -1)
        {
            if(errno == EAGAIN ||
               errno == EWOULDBLOCK)
            {
                return;
            }

            throw std::runtime_error(
                std::string("accept failed: ") +
                strerror(errno)
            );
        }

        setNonBlocking(client_fd);

        auto connection =
            std::make_unique<Connection>(
                client_fd,
                processor
            );

        epoll_event event{};

        event.events =
            EPOLLIN |
            EPOLLRDHUP |
            EPOLLET;

        event.data.ptr =
            connection.get();

        if(
            epoll_ctl(
                epoll_fd,
                EPOLL_CTL_ADD,
                client_fd,
                &event
            ) == -1
        )
        {
            close(client_fd);

            throw std::runtime_error(
                std::string("epoll_ctl ADD failed: ") +
                strerror(errno)
            );
        }

        connections.emplace(
            client_fd,
            std::move(connection)
        );
    }
}


void RedisServer::eventLoop()
{
    constexpr int MAX_EVENTS = 64;

    epoll_event events[MAX_EVENTS];

    while(true)
    {
        int ready =
            epoll_wait(
                epoll_fd,
                events,
                MAX_EVENTS,
                -1
            );

        if(ready == -1)
        {
            if(errno == EINTR)
            {
                continue;
            }

            throw std::runtime_error(
                std::string("epoll_wait failed: ") +
                strerror(errno)
            );
        }

        for(int i = 0; i < ready; i++)
        {
            if(events[i].data.fd == server_fd)
            {
                acceptConnections();
            }
            else
            {
                Connection* connection =
                    static_cast<Connection*>(
                        events[i].data.ptr
                    );

                handleClientEvent(
                    *connection,
                    events[i].events
                );
            }
        }
    }
}

void RedisServer::updateEvents(
    Connection& connection,
    uint32_t events)
{
    epoll_event event{};

    event.events = events;

    event.data.ptr = &connection;

    if(
        epoll_ctl(
            epoll_fd,
            EPOLL_CTL_MOD,
            connection.getSocket(),
            &event
        ) == -1)
    {
        throw std::runtime_error(
            "epoll_ctl MOD failed"
        );
    }
}

void RedisServer::removeConnection(
    Connection& connection)
{
    epoll_ctl(
        epoll_fd,
        EPOLL_CTL_DEL,
        connection.getSocket(),
        nullptr
    );

    connections.erase(
        connection.getSocket()
    );
}

void RedisServer::handleClientEvent(
    Connection& connection,
    uint32_t events)
{
    if(events &
        (EPOLLERR |
         EPOLLHUP |
         EPOLLRDHUP))
    {
        removeConnection(connection);
        return;
    }

    if(events & EPOLLIN)
    {
        ReceiveResult result =
            connection.receive();

        if(result ==
            ReceiveResult::Disconnected)
        {
            removeConnection(connection);
            return;
        }

        connection.processRequests();

        if(connection.hasPendingOutput())
        {
            FlushResult result = connection.flush();

            switch(result)
            {
                case FlushResult::Complete:
                    break;

                case FlushResult::Pending:
                    updateEvents(
                        connection,
                        EPOLLIN |
                        EPOLLOUT |
                        EPOLLRDHUP |
                        EPOLLET
                    );
                    break;

                case FlushResult::Disconnected:
                    removeConnection(connection);
                    return;
            }
        }
    }

    if(events & EPOLLOUT)
    {
        FlushResult result =
            connection.flush();

        switch(result)
        {
            case FlushResult::Complete:

                updateEvents(
                    connection,
                    EPOLLIN |
                    EPOLLRDHUP |
                    EPOLLET
                );

                break;

            case FlushResult::Pending:

                break;

            case FlushResult::Disconnected:

                removeConnection(connection);

                return;
        }
    }
}