add_library(
    executor

    CommandExecutor.cpp
)

target_include_directories(
    executor
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}

    ${PROJECT_SOURCE_DIR}/src/datastore

    ${PROJECT_SOURCE_DIR}/src/common
)

target_link_libraries(
    executor
    PUBLIC
    datastore
)