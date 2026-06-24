#include "RedisServer.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

RedisServer::RedisServer(int port): port(port),server_fd(-1),executor(datastore){

}
void RedisServer::start(){
    initializeSocket();

    acceptClients();
}
void RedisServer::initializeSocket(){
    server_fd =
        socket(
            AF_INET,
            SOCK_STREAM,
            0
        );

    sockaddr_in addr{};

    addr.sin_family = AF_INET;

    addr.sin_port =
        htons(port);

    addr.sin_addr.s_addr =
        INADDR_ANY;

    bind(
        server_fd,
        (sockaddr*)&addr,
        sizeof(addr)
    );

    listen(server_fd, 5);
}
void RedisServer::acceptClients(){
    while(true)
    {
        int client_fd =
            accept(
                server_fd,
                nullptr,
                nullptr
            );

        handleClient(client_fd);
    }
}
void RedisServer::handleClient(int client_fd){
    char buffer[1024];

    int bytes =
        recv(
            client_fd,
            buffer,
            sizeof(buffer)-1,
            0
        );

    if(bytes <= 0)
        return;

    buffer[bytes] = '\0';

    std::string input(buffer);

    Command cmd =
        parser.parse(input);

    std::string response =
        executor.execute(cmd);

    response += "\n";

    send(
        client_fd,
        response.c_str(),
        response.size(),
        0
    );

    close(client_fd);
}