#include "server/RedisServer.h"

int main()
{
    RedisServer server(6380);

    server.start();
}