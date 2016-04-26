cmake_minimum_required(VERSION 3.4)
project(raven_lang LANGUAGES CXX)

list(APPEND WARNING_OPTIONS -Wall -Wextra -Weffc++)
if(${CMAKE_CXX_COMPILER} MATCHES "clang\\+\\+")
    list(APPEND WARNING_OPTIONS -Wpedantic)
elseif(CMAKE_COMPILER_IS_GNUCXX)
    list(APPEND WARNING_OPTIONS -pedantic)
endif()

add_compile_options(${WARNING_OPTIONS} -std=c++14 -pipe)

set(SOURCE_FILES main.cpp)
add_executable(raven_lang ${SOURCE_FILES})

add_subdirectory(compiler)
