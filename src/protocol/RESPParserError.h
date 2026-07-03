#pragma once

#include <stdexcept>
#include <string>

class RESPParserError : public std::runtime_error
{
public:
    explicit RESPParserError(const std::string& message)
        : std::runtime_error(message)
    {
    }
};