cmake_minimum_required(VERSION 3.4)
project(raven_lang LANGUAGES CXX)

list(APPEND WARNING_OPTIONS -Wall -Wextra -Weffc++)
if(${CMAKE_CXX_COMPILER} MATCHES "clang\\+\\+")
    list(APPEND WARNING_OPTIONS -Wpedantic)
elseif(CMAKE_COMPILER_IS_GNUCXX)
    list(APPEND WARNING_OPTIONS -pedantic)
endif()

add_compile_options(${WARNING_OPTIONS} -std=c++14 -pipe)

find_package(Boost
        REQUIRED COMPONENTS program_options system)

find_package(Doxygen)
if(DOXYGEN_FOUND)
    set(DOXYGEN_CONFIG ${CMAKE_CURRENT_SOURCE_DIR}/raven-language.doxyfile)
    set(DOXYGEN_OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/doc)

    add_custom_command(
            OUTPUT ${DOXYGEN_OUTPUT}
            COMMAND ${CMAKE_COMMAND} -E echo_append "Building API Documentation..."
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_CONFIG}
            COMMAND ${CMAKE_COMMAND} -E echo "Done."
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            DEPENDS ${DOXYGEN_CONFIG}
    )

    add_custom_target(apidoc ALL DEPENDS ${DOXYGEN_OUTPUT})

    add_custom_target(
            apidoc_forced
            COMMAND ${CMAKE_COMMAND} -E echo_append "Building API Documentation..."
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_INPUT}
            COMMAND ${CMAKE_COMMAND} -E echo "Done."
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
endif()

set(SOURCE_FILES main.cpp)
add_executable(raven_lang ${SOURCE_FILES})

add_subdirectory(compiler)

