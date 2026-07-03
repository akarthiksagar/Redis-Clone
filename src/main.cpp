#include "server/RedisServer.h"
#include <iostream>

int main()
{
    try
    {
        RedisServer server(6380);

        server.start();
    }
    catch(const std::exception& e)
    {
        std::cerr
            << e.what()
            << '\n';
    }
}