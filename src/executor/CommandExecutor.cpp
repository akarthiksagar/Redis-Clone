#include "CommandExecutor.h"

CommandExecutor::CommandExecutor(DataStore& ds)
    : datastore(ds)
{
}

Response CommandExecutor::execute(
    const Command& cmd)
{
    switch(cmd.type)
    {
        case CommandType::PING: {
            return {
                ResponseType::SimpleString,
                "PONG"
            };
        }

        case CommandType::SET: {
            if(cmd.args.size() != 2)
                return {
                    ResponseType::Error,
                    "wrong number of arguments"
                };

            datastore.set(
                cmd.args[0],
                cmd.args[1]
            );

            return {
                ResponseType::SimpleString,
                "OK"
            };
        }

        case CommandType::GET: {
            if(cmd.args.size() != 1)
                return {
                    ResponseType::Error,
                    "wrong number of arguments"
                };

            auto value = datastore.get(cmd.args[0]);
            if(!value)
            {
                return {
                    ResponseType::NullBulkString,
                    ""
                };
            }
            return {
                ResponseType::BulkString,
                *value
            };
        }

        case CommandType::DEL:{
            if(cmd.args.size() != 1)
                return {
                    ResponseType::Error,
                    "wrong number of arguments"
                };

            return {
                ResponseType::Integer,
                datastore.del(cmd.args[0]) ? 1:0
            };
        }

        case CommandType::EXISTS: {
            if(cmd.args.size() != 1)
                return {
                    ResponseType::Error,
                    "wrong number of arguments"
                };
            return {
                ResponseType::Integer,
                datastore.exists(cmd.args[0]) ? 1 : 0
            };
        }

        default:
            return {
                ResponseType::Error,
                "Unknown command"
            };
    }
}