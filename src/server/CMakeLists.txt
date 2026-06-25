add_library(
    server

    RedisServer.cpp
)

target_include_directories(
    server
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}

    ${PROJECT_SOURCE_DIR}/src/parser

    ${PROJECT_SOURCE_DIR}/src/executor

    ${PROJECT_SOURCE_DIR}/src/datastore

    ${PROJECT_SOURCE_DIR}/src/common
)

target_link_libraries(
    server
    PUBLIC
    parser
    executor
    datastore
    Threads::Threads
)