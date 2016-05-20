cmake_minimum_required(VERSION 3.4)
project(raven_lang LANGUAGES CXX)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++")

list(APPEND WARNING_OPTIONS -Wall -Wextra -Weffc++)
if(${CMAKE_CXX_COMPILER} MATCHES "clang\\+\\+")
    list(APPEND WARNING_OPTIONS -Wpedantic)
elseif(CMAKE_COMPILER_IS_GNUCXX)
    list(APPEND WARNING_OPTIONS -pedantic)
endif()

add_compile_options(${WARNING_OPTIONS} -pipe -pthread)
if(${CMAKE_CXX_COMPILER} MATCHES "clang\\+\\+")
    add_compile_options(-std=c++1z -stdlib=libc++)
    # add_compile_options(-stdlib=libc++ -std=c++1z)
    # set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lc++abi")
else()
    add_compile_options(-std=c++14)
endif()

find_package(Boost
        REQUIRED COMPONENTS program_options system)

include(CheckIncludeFiles)

# Check for new C++14 and C++17 features used by the compiler
check_include_files(any HAVE_HEADER_ANY)
check_include_files(experimental/any HAVE_HEADER_EXPERIMENTAL_ANY)

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
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_CONFIG}
            COMMAND ${CMAKE_COMMAND} -E echo "Done."
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
endif()

set(SOURCE_FILES main.cpp)
add_executable(raven_lang ${SOURCE_FILES})

add_subdirectory(compiler)

configure_file(autoconf.h.in ${CMAKE_SOURCE_DIR}/autoconf.h)
