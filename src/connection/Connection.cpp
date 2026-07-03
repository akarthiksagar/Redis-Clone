#include "Connection.h"
#include <sys/socket.h>   // recv(), send()
#include <unistd.h>        // close()
#include <cerrno>
Connection::Connection(
    int client_fd,
    RequestProcessor& processor
)
    :
    client_fd(client_fd),
    processor(processor)
{
}
// bool Connection::receive()
// {
//     char buffer[1024];

//     int bytes = recv(
//         client_fd,
//         buffer,
//         sizeof(buffer),
//         0
//     );

//     if(bytes <= 0)
//         return false;

//     inputBuffer.append(
//         buffer,
//         bytes
//     );

//     return true;
// }

// bool Connection::sendResponse(
//     const std::string& response)
// {
//     size_t totalSent = 0;

//     while(totalSent < response.size())
//     {
//         ssize_t bytes =
//             send(
//                 client_fd,
//                 response.c_str() + totalSent,
//                 response.size() - totalSent,
//                 0
//             );

//         if(bytes <= 0)
//         {
//             return false;
//         }

//         totalSent += bytes;
//     }

//     return true;
// }

// void Connection::process()
// {
//     while(true)
//     {
//         if(!receive())
//             break;

//         while(true)
//         {
//             std::string response;

//             ParseResult result =
//                 processor.process(
//                     inputBuffer,
//                     response
//                 );

//             if(result == ParseResult::Incomplete)
//                 break;

//             if(!sendResponse(response))
//                 return;
//         }
//     }

//     close(client_fd);
// }
Connection::~Connection()
{
    close(client_fd);
}
bool Connection::hasPendingOutput() const
{
    return !outputBuffer.empty();
}

int Connection::getSocket() const
{
    return client_fd;
}


ReceiveResult Connection::receive()
{
    char buffer[4096];

    while(true)
    {
        ssize_t bytes =
            recv(
                client_fd,
                buffer,
                sizeof(buffer),
                0
            );

        if(bytes > 0)
        {
            inputBuffer.append(
                buffer,
                bytes
            );
        }
        else if(bytes == 0)
        {
            // Client closed connection
            return ReceiveResult::Disconnected;
        }
        else
        {
            if(errno == EAGAIN ||
               errno == EWOULDBLOCK)
            {
                // Socket completely drained
                return ReceiveResult::Success;
            }

            return ReceiveResult::Disconnected;
        }
    }
}
void Connection::processRequests()
{
    while(true)
    {
        std::string response;

        ProcessResult result =
            processor.process(
                inputBuffer,
                response
            );

        switch(result)
        {
            case ProcessResult::Processed:

                outputBuffer += response;
                break;

            case ProcessResult::NeedMoreData:

                return;

            case ProcessResult::Disconnect:

                throw std::runtime_error(
                    "Disconnect"
                );
        }
    }
}

FlushResult Connection::flush()
{
    while (outputOffset < outputBuffer.size())
    {
        ssize_t bytes =
            send(
                client_fd,
                outputBuffer.data() + outputOffset,
                outputBuffer.size() - outputOffset,
                0
            );

        if (bytes > 0)
        {
            outputOffset += bytes;
        }
        else if (bytes == 0)
        {
            return FlushResult::Disconnected;
        }
        else
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                // Socket send buffer is full.
                // Wait for the next EPOLLOUT event.
                return FlushResult::Pending;
            }

            return FlushResult::Disconnected;
        }
    }

    // Everything has been sent.
    outputBuffer.clear();
    outputOffset = 0;

    return FlushResult::Complete;
}
