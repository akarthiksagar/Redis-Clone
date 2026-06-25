#pragma once

#include <string>

#include "../common/Command.h"
#include "../datastore/DataStore.h"

class CommandExecutor {
private:
    DataStore& datastore;

public:
    CommandExecutor(DataStore& ds);

    std::string execute(const Command& cmd);
};