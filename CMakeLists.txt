cmake_minimum_required(VERSION 3.4)
project(raven_lang)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
add_executable(raven_lang ${SOURCE_FILES})