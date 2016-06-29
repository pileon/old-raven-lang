project(testing CXX C)

add_subdirectory(googletest)

include_directories(${gtest_SOURCE_DIR}/include)

add_subdirectory(compiler)

add_custom_target(build_tests
        DEPENDS lexer_test
        )

add_custom_target(run_tests
        DEPENDS build_tests
        COMMAND ${CMAKE_BINARY_DIR}/tests/compiler/lexer/lexer_test
        )
