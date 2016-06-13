set(SOURCE_FILES buffer.h lexer.h token.h)
#add_library(lexer ${SOURCE_FILES})
#
add_executable(test_lexer test_lexer.cpp ${SOURCE_FILES})
#target_link_libraries(test_lexer lexer)
