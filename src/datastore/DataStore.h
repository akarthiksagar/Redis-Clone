#pragma once

#include <string>
#include <unordered_map>
#include <shared_mutex>
#include <mutex>

class DataStore {
private:
    std::unordered_map<std::string, std::string> db;
    mutable std::shared_mutex mtx;
public:
    void set(const std::string& key, const std::string& value);

    std::string get(const std::string& key) const;

    bool del(const std::string& key);

    bool exists(const std::string& key) const;
};