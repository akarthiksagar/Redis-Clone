#pragma once

#include <string>
#include <vector>

enum class CommandType {
    SET,
    GET,
    DEL,
    EXISTS,
    PING,
    INVALID
};

struct Command {
    CommandType type;
    std::vector<std::string> args;
};