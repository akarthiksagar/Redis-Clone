#include "CommandParser.h"

#include <sstream>
#include <vector>
#include <iostream>

Command CommandParser::parse(const std::string& input)
{
    std::stringstream ss(input);

    std::vector<std::string> tokens;

    std::string token;

    while (ss >> token)
    {
        tokens.push_back(token);
    }

    return parse(tokens);
}

Command CommandParser::parse(const std::vector<std::string>& tokens)
{
    Command cmd;
    if(tokens.empty()){
        cmd.type = CommandType::INVALID;
        return cmd;
    }
    std::string commandName=tokens[0];
    if(commandName == "SET") cmd.type = CommandType::SET;
    else if(commandName == "GET") cmd.type = CommandType::GET;
    else if(commandName == "DEL") cmd.type = CommandType::DEL;
    else if(commandName == "EXISTS") cmd.type = CommandType::EXISTS;
    else if(commandName == "PING") cmd.type = CommandType::PING;
    else cmd.type = CommandType::INVALID;
    std::string token;
    for(std::string token:tokens){
        if(token==commandName) continue;
        cmd.args.push_back(token);
    }
    return cmd;
}