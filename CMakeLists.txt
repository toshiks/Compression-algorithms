cmake_minimum_required(VERSION 3.6)
project(INF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp files.h files.cpp)
add_executable(INF ${SOURCE_FILES})