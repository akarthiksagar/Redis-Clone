#include "RequestProcessor.h"

#include <vector>

RequestProcessor::RequestProcessor(
    CommandParser& commandParser,
    RESPParser& respParser,
    RESPEncoder& respEncoder,
    CommandExecutor& executor
)
    : commandParser(commandParser),
      respParser(respParser),
      respEncoder(respEncoder),
      executor(executor)
{
}

bool RequestProcessor::isRESP(
    const std::string& input) const
{
    return !input.empty() &&
           input.front() == '*';
}

std::string RequestProcessor::protocolError() const
{
    return respEncoder.encode(
        {
            ResponseType::Error,
            std::string("Protocol Error")
        }
    );
}
ProcessResult RequestProcessor::process(
    std::string& buffer,
    std::string& response)
{
    std::vector<std::string> tokens;

    Command cmd;
    if(buffer.empty())
    {
        return ProcessResult::NeedMoreData;
    }
    if(isRESP(buffer))
    {
        try
        {
            ParseResult result =
                respParser.parse(
                    buffer,
                    tokens
                );

            if(result == ParseResult::Incomplete)
                return ProcessResult::NeedMoreData;
             
            cmd =
            commandParser.parse(tokens);
        }
        catch(const RESPParserError& e)
        {
            buffer.clear();
            Response error
            {
                ResponseType::Error,
                std::string(e.what())
            };

            response =
                respEncoder.encode(error);

            return ProcessResult::Processed;
        }
    }
    else
    {
        cmd =
            commandParser.parse(buffer);

        buffer.clear();
    }
    Response redisResponse =
    executor.execute(cmd);
    response =
        respEncoder.encode(redisResponse);

    return ProcessResult::Processed;
}