#pragma once

#include <string>

#include "../parser/CommandParser.h"
#include "../protocol/RESPParser.h"
#include "../protocol/RESPEncoder.h"
#include "../protocol/RESPParserError.h"
#include "../executor/CommandExecutor.h"

enum class ProcessResult
{
    Processed,
    NeedMoreData,
    Disconnect
};

class RequestProcessor
{
private:
    CommandParser& commandParser;
    RESPParser& respParser;
    RESPEncoder& respEncoder;
    CommandExecutor& executor;

public:
    RequestProcessor(
        CommandParser& commandParser,
        RESPParser& respParser,
        RESPEncoder& respEncoder,
        CommandExecutor& executor
    );
    bool isRESP(const std::string& input) const;
    std::string protocolError() const;
    ProcessResult process(
        std::string& inputBuffer,
        std::string& response
    );
};