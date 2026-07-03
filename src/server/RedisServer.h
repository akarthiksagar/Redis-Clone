#pragma once

#include "../datastore/DataStore.h"
#include "../parser/CommandParser.h"
#include "../executor/CommandExecutor.h"

#include "../protocol/RESPParser.h"
#include "../protocol/RESPEncoder.h"
#include "../request/RequestProcessor.h"
#include "../connection/Connection.h"
#include "../network/SocketUtils.h"
#include <unordered_map>
#include <memory>


class RedisServer
{
public:
    RedisServer(int port);
    ~RedisServer();
    void start();

private:

    // Socket setup
    void initializeSocket();

    // epoll setup
    void initializeEpoll();

    // Main server loop
    void eventLoop();

    // Accept every pending client
    void acceptConnections();

    // Handle one epoll event
    void handleClientEvent(
        Connection& connection,
        uint32_t events
    );

    void updateEvents(
        Connection& connection,
        uint32_t events
    );
    void removeConnection(
        Connection& connection
    );

private:
    int port;

    int server_fd;

    int epoll_fd;

    DataStore datastore;

    CommandParser commandParser;

    RESPParser respParser;

    RESPEncoder respEncoder;

    CommandExecutor executor;

    RequestProcessor processor;

    std::unordered_map<
        int,
        std::unique_ptr<Connection>
    > connections;
};