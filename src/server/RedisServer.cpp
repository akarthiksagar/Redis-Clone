#include "RedisServer.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <thread>

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
        std::thread(&RedisServer::handleClient,this,client_fd).detach();
    }
}
void RedisServer::handleClient(int client_fd){
    std::cout<<"Connected to client with client_fd: "<<client_fd<<'\n';
    while(true){
        char buffer[1024];
        int bytes =
            recv(
                client_fd,
                buffer,
                sizeof(buffer)-1,
                0
            );

        if(bytes <= 0)
            break;

        buffer[bytes] = '\0';

        std::string input(buffer);
        while(!input.empty() &&(input.back() == '\n' || input.back() == '\r')) {
            input.pop_back();
        }
        if(input=="EXIT"||input=="exit") break;
        
        std::cout << "Received: " << input << '\n';

        Command cmd =
            parser.parse(input);

        std::cout << "Parsed\n";

        std::string response =
            executor.execute(cmd);

        std::cout << "Executed\n";

        response += "\n";

        std::cout << "Sending: " << response;

        send(
            client_fd,
            response.c_str(),
            response.size(),
            0
        );

        std::cout << "Sent\n";
        
    }
    std::cout<<"Disonnected to client with client_fd: "<<client_fd<<'\n';
    close(client_fd);
}