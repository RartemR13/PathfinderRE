cmake_minimum_required(VERSION 3.10)

project(PathfinderRE)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O3")

add_executable(Pathfinder Pathfinder.cpp)
add_dependencies(Pathfinder vis XPSTest VoyagerSolver run_test)

cmake_policy(SET CMP0074 NEW)
find_package(Boost)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS}) 
    link_directories(${Boost_LIBRARY_DIRS})
    target_link_libraries(Pathfinder ${Boost_LIBRARIES})
endif()

add_subdirectory(RArea)
target_link_libraries(Pathfinder PUBLIC RArea)
target_include_directories(Pathfinder PUBLIC RArea)

add_subdirectory(XPSHelper)
target_link_libraries(Pathfinder PUBLIC XPSHelper)
target_include_directories(Pathfinder PUBLIC XPSHelper)

add_subdirectory(KLines)
target_link_libraries(Pathfinder PUBLIC KLines)
target_include_directories(Pathfinder PUBLIC KLines)

add_subdirectory(TimeChecker)
target_link_libraries(Pathfinder PUBLIC TimeChecker)
target_include_directories(Pathfinder PUBLIC TimeChecker)

add_subdirectory(G3)
target_link_libraries(Pathfinder PUBLIC G3)
target_include_directories(Pathfinder PUBLIC G3)

add_subdirectory(PathHelper)
target_link_libraries(Pathfinder PUBLIC PathHelper)
target_include_directories(Pathfinder PUBLIC PathHelper)

add_custom_target(vis
	${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/vis.py ${CMAKE_CURRENT_BINARY_DIR}/vis.py
)

add_custom_target(XPSTest
	${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/XPSTest ${CMAKE_CURRENT_BINARY_DIR}/XPSTest 
)

add_custom_target(VoyagerSolver
	${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/VoyagerSolver ${CMAKE_CURRENT_BINARY_DIR}/VoyagerSolver
)

add_custom_target(run_test
	${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/run_test.bat ${CMAKE_CURRENT_BINARY_DIR}/run_test.bat
)