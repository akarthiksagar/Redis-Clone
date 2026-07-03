#pragma once

#include <string>

#include "../request/RequestProcessor.h"

enum class ReceiveResult
{
    Success,
    Disconnected
};
enum class FlushResult
{
    Complete,
    Pending,
    Disconnected
};
class Connection
{
private:
    int client_fd;

    std::string inputBuffer;

    std::string outputBuffer;
    size_t outputOffset = 0;

    RequestProcessor& processor;

public:
    Connection(
        int client_fd,
        RequestProcessor& processor
    );

    ~Connection();

    // Read all available data from the socket
    ReceiveResult receive();

    // Process every complete request currently present
    void processRequests();

    // Flush pending output to the socket
    FlushResult flush();

    // Returns true if outputBuffer is not empty
    bool hasPendingOutput() const;

    // Socket descriptor
    int getSocket() const;
};