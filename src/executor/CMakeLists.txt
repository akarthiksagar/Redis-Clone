add_library(
    executor

    CommandExecutor.cpp
)

target_include_directories(
    executor
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
    executor
    PUBLIC
    datastore
    common
)