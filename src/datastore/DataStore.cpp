#include "DataStore.h"

void DataStore::set(const std::string& key,const std::string& value){
    std::unique_lock lock(mtx);
    db[key] = value;
}

std::string DataStore::get(const std::string& key) const{
    std::shared_lock lock(mtx);
    auto it = db.find(key);

    if(it == db.end())
        return "(nil)";

    return it->second;
}

bool DataStore::del(const std::string& key){
    std::unique_lock lock(mtx);
    return db.erase(key);
}

bool DataStore::exists(const std::string& key) const{
    std::shared_lock lock(mtx);
    return db.find(key) != db.end();
}