#pragma once

#include <string>
#include <vector>

enum class ParseResult
{
    Complete,
    Incomplete,
    Error
};
class RESPParser
{
public:
    ParseResult parse(
        std::string& buffer,
        std::vector<std::string>& tokens
    );

private:
    bool readLine(
        const std::string& buffer,
        size_t& pos,
        std::string& line
    ) const;

    bool readBulkString(
        const std::string& buffer,
        size_t& pos,
        std::string& value
    ) const;
};