#pragma once

#include <string>
#include <variant>

enum class ResponseType
{
    SimpleString,
    BulkString,
    Integer,
    Error,
    NullBulkString
};

struct Response
{
    ResponseType type;
    std::variant<
        std::monostate,
        std::string,
        int
    > value;
};