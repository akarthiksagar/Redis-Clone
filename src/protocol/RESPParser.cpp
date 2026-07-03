#include "RESPParser.h"
#include "RESPParserError.h"

#include <sstream>

bool RESPParser::readLine(
    const std::string& buffer,
    size_t& pos,
    std::string& line
) const
{
    size_t end =
        buffer.find("\r\n", pos);

    if(end == std::string::npos)
    {
        return false;
    }

    line =
        buffer.substr(
            pos,
            end - pos
        );

    pos = end + 2;

    return true;
}
bool RESPParser::readBulkString(
    const std::string& buffer,
    size_t& pos,
    std::string& value
) const
{
    std::string line;

    if(!readLine(buffer,pos,line))
    {
        return false;
    }

    if(line.empty() || line[0] != '$')
    {
        throw RESPParserError("Expected bulk string");
    }

    int length;

    try
    {
        length =
            std::stoi(
                line.substr(1)
            );
    }
    catch(...)
    {
        throw RESPParserError("Invalid bulk string length");
    }

    if(pos + length + 2 > buffer.size())
    {
        return false;
    }

    value =
        buffer.substr(
            pos,
            length
        );

    if(buffer[pos+length] != '\r' ||
       buffer[pos+length+1] != '\n')
    {
        throw RESPParserError(
            "Missing CRLF"
        );
    }

    pos += length + 2;

    return true;
}
ParseResult RESPParser::parse(
    std::string& buffer,
    std::vector<std::string>& tokens)
{
    tokens.clear();

    size_t pos = 0;

    std::string line;

    if(!readLine(buffer,pos,line))
    {
        return ParseResult::Incomplete;
    }

    if(line.empty() || line[0] != '*')
    {
        throw RESPParserError(
            "Expected Array"
        );
    }

    int count;

    try
    {
        count =
            std::stoi(
                line.substr(1)
            );
    }
    catch(...)
    {
        throw RESPParserError(
            "Invalid Array Length"
        );
    }

    for(int i=0;i<count;i++)
    {
        std::string value;

        if(!readBulkString(
                buffer,
                pos,
                value))
        {
            return ParseResult::Incomplete;
        }

        tokens.push_back(value);
    }

    buffer.erase(
        0,
        pos
    );

    return ParseResult::Complete;
}