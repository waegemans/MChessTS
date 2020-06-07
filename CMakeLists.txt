cmake_minimum_required(VERSION 3.13)
project(MChessTS)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

find_package(GTest)
if (NOT GTEST_FOUND)
    message(STATUS "Adding GTest as external project")
    include(googletest)
endif ()

add_subdirectory(lib)
add_subdirectory(src)
add_subdirectory(test)