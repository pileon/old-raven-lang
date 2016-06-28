set(SOURCE_FILES buffer.cpp)
add_executable(lexer_test ${SOURCE_FILES})
target_link_libraries(lexer_test gtest gtest_main)
