set(SOURCE_FILES lexer.h buffer.h token.cpp token.h)
add_library(lexer ${SOURCE_FILES})

add_executable(test_lexer test_lexer.cpp)
target_link_libraries(test_lexer lexer)
