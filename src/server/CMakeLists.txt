add_library(
    server

    RedisServer.cpp
)

target_include_directories(
    server
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
    server
    PUBLIC
    parser
    executor
    datastore
    connection
    network
    protocol
    request
    Threads::Threads
)