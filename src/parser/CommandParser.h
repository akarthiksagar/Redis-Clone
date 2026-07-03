#pragma once

#include <string>
#include "../common/Command.h"

class CommandParser {
public:
    Command parse(const std::string& input);
    Command parse(const std::vector<std::string>& tokens);

};