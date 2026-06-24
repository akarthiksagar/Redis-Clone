#include "CommandParser.h"

#include <sstream>
#include <vector>

Command CommandParser::parse(const std::string& input)
{
    std::stringstream ss(input);

    std::string commandName;
    ss >> commandName;

    Command cmd;

    if(commandName == "SET")
        cmd.type = CommandType::SET;
    else if(commandName == "GET")
        cmd.type = CommandType::GET;
    else if(commandName == "DEL")
        cmd.type = CommandType::DEL;
    else if(commandName == "EXISTS")
        cmd.type = CommandType::EXISTS;
    else if(commandName == "PING")
        cmd.type = CommandType::PING;
    else
        cmd.type = CommandType::INVALID;

    std::string token;

    while(ss >> token)
    {
        cmd.args.push_back(token);
    }

    return cmd;
}