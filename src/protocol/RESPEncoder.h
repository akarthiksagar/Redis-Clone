#pragma once

#include <string>

#include "../common/Response.h"

class RESPEncoder
{
public:
    std::string encode(const Response& response) const;

private:
    std::string simpleString(const std::string& value) const;

    std::string error(const std::string& message) const;

    std::string bulkString(const std::string& value) const;

    std::string nullBulkString() const;

    std::string integer(int value) const;
};