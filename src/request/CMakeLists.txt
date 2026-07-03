add_library(
    request
    RequestProcessor.cpp
)

target_include_directories(
    request
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(
    request
    PUBLIC
    parser
    protocol
    executor
)