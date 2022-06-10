cmake_minimum_required(VERSION 3.10)

add_library(XPSHelper XPSHelper.cpp)
target_link_libraries(XPSHelper PUBLIC RArea)
target_include_directories(XPSHelper PUBLIC RArea)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O3")

cmake_policy(SET CMP0074 NEW)
find_package(Boost)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS}) 
    link_directories(${Boost_LIBRARY_DIRS})
    target_link_libraries(XPSHelper ${Boost_LIBRARIES})
endif()