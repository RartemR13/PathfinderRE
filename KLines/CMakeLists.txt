cmake_minimum_required(VERSION 3.10)

add_library(KLines KLines.cpp)
target_link_libraries(KLines PUBLIC XPSHelper)
target_include_directories(KLines PUBLIC XPSHelper)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O3")