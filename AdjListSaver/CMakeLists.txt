cmake_minimum_required(VERSION 3.10)

add_library(AdjListSaver AdjListSaver.cpp)
add_subdirectory(SHA256)

target_link_libraries(AdjListSaver PUBLIC sha256)
target_include_directories(AdjListSaver PUBLIC sha256)