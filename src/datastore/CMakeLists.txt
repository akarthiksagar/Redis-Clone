add_library(
    datastore

    DataStore.cpp
)

target_include_directories(
    datastore
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)