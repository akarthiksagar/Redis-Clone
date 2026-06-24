#pragma once

#include "../datastore/DataStore.h"
#include "../parser/CommandParser.h"
#include "../executor/CommandExecutor.h"

class RedisServer
{
private:
    int port;
    int server_fd;

    DataStore datastore;
    CommandParser parser;
    CommandExecutor executor;

public:
    RedisServer(int port);

    void start();

private:
    void initializeSocket();

    void acceptClients();

    void handleClient(int client_fd);
};