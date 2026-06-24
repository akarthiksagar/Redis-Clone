#include "CommandExecutor.h"

CommandExecutor::CommandExecutor(DataStore& ds)
    : datastore(ds)
{
}

std::string CommandExecutor::execute(
    const Command& cmd)
{
    switch(cmd.type)
    {
        case CommandType::PING:
            return "PONG";

        case CommandType::SET:
        {
            if(cmd.args.size() != 2)
                return "ERR wrong number of arguments";

            datastore.set(
                cmd.args[0],
                cmd.args[1]
            );

            return "OK";
        }

        case CommandType::GET:
        {
            if(cmd.args.size() != 1)
                return "ERR wrong number of arguments";

            return datastore.get(
                cmd.args[0]
            );
        }

        case CommandType::DEL:
        {
            if(cmd.args.size() != 1)
                return "ERR wrong number of arguments";

            return datastore.del(
                cmd.args[0]
            ) ? "1" : "0";
        }

        case CommandType::EXISTS:
        {
            if(cmd.args.size() != 1)
                return "ERR wrong number of arguments";

            return datastore.exists(
                cmd.args[0]
            ) ? "1" : "0";
        }

        default:
            return "ERR unknown command";
    }
}