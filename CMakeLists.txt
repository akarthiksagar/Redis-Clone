cmake_minimum_required(VERSION 3.20)

project(
    RedisClone
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

add_subdirectory(src/common)
add_subdirectory(src/datastore)
add_subdirectory(src/parser)
add_subdirectory(src/protocol)  
add_subdirectory(src/executor)
add_subdirectory(src/request)
add_subdirectory(src/connection)
add_subdirectory(src/network)
add_subdirectory(src/server)

add_executable(
    redis_clone
    src/main.cpp
    src/protocol/RESPEncoder.cpp
)

target_link_libraries(
    redis_clone
    PRIVATE
    server
)