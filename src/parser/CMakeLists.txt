add_library(
    parser

    CommandParser.cpp
)

target_include_directories(
    parser
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_include_directories(
    parser
    PUBLIC
    ${PROJECT_SOURCE_DIR}/src/common
)