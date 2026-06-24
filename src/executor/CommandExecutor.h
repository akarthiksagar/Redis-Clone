#pragma once

#include <string>

#include "Command.h"
#include "DataStore.h"

class CommandExecutor {
private:
    DataStore& datastore;

public:
    CommandExecutor(DataStore& ds);

    std::string execute(const Command& cmd);
};