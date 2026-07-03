#include "RESPEncoder.h"
std::string RESPEncoder::simpleString(
    const std::string& value) const
{
    return "+" + value + "\r\n";
}
std::string RESPEncoder::error(
    const std::string& message) const
{
    return "-ERR " + message + "\r\n";
}
std::string RESPEncoder::bulkString(
    const std::string& value) const
{
    return "$"
        + std::to_string(value.size())
        + "\r\n"
        + value
        + "\r\n";
}
std::string RESPEncoder::nullBulkString() const
{
    return "$-1\r\n";
}
std::string RESPEncoder::integer(
    int value) const
{
    return ":"
        + std::to_string(value)
        + "\r\n";
}

std::string RESPEncoder::encode(
    const Response& response) const
{
    switch(response.type)
    {
        case ResponseType::SimpleString:
            return simpleString(
                std::get<std::string>(response.value)
            );

        case ResponseType::BulkString:
            return bulkString(
                std::get<std::string>(response.value)
            );

        case ResponseType::Integer:
            return integer(
                std::get<int>(response.value)
            );

        case ResponseType::Error:
            return error(
                std::get<std::string>(response.value)
            );

        case ResponseType::NullBulkString:
            return nullBulkString();
    }

    return error("Unknown response type");
}