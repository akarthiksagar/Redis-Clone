#include "DataStore.h"

void DataStore::set(const std::string& key,
                    const std::string& value)
{
    db[key] = value;
}

std::string DataStore::get(
    const std::string& key) const
{
    auto it = db.find(key);

    if(it == db.end())
        return "(nil)";

    return it->second;
}

bool DataStore::del(
    const std::string& key)
{
    return db.erase(key);
}

bool DataStore::exists(
    const std::string& key) const
{
    return db.find(key) != db.end();
}