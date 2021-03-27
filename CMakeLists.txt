cmake_minimum_required(VERSION 3.12)
project(MChessTS)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror -pthread")
set(CMAKE_CXX_FLAGS_DEBUG "-g3 -fsanitize=address")
set(CMAKE_CXX_FLAGS_RELEASE  "-O3")

find_package(GTest)
if (NOT GTEST_FOUND)
    message(STATUS "Adding GTest as external project")
    include(googletest)
endif ()

add_subdirectory(lib)
add_subdirectory(src)
add_subdirectory(test)