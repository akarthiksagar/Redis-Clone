add_library(protocol
    RESPParser.cpp
    RESPEncoder.cpp
)

target_include_directories(protocol
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)