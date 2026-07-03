#pragma once

#include <string>

#include "../common/Command.h"
#include "../common/Response.h"
#include "../datastore/DataStore.h"

class CommandExecutor {
private:
    DataStore& datastore;

public:
    CommandExecutor(DataStore& ds);

    Response execute(const Command& cmd);
};