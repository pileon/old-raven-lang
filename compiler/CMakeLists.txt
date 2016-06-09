set(SOURCE_FILES diagnostics.h)
add_executable(test_diagnostics test_diagnostics.cpp)
target_compile_definitions(test_diagnostics PRIVATE -DTESTING=1)
add_subdirectory(lexer)
