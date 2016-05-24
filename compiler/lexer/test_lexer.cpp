//
// Created by joachimp on 2016-05-16.
//

#include <iostream>
#include <memory>
#include "buffer.h"
#include "lexer.h"
#include "token.h"

namespace l = compiler::lexer;
namespace b = l::buffers;
// namespace t = compiler::tokens;

namespace
{
    void test_lexer_buffer()
    {
        {
            std::unique_ptr<b::buffer> buffer{new b::string_buffer{"Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.", "foo"}};

            char ch;
            while ((ch = buffer->get()) != b::buffer::end)
            {
                std::cout << ch;
            }

            std::cout << '\n';
        }

        {
            std::unique_ptr<b::buffer> buffer{new b::string_buffer{ "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.", "foo" }};

            std::cout << "First character       : " << buffer->get() << '\n';
            std::cout << "Second character      : " << buffer->get() << '\n';
            buffer->unget();
            std::cout << "Second character again: " << buffer->get() << '\n';
            buffer->putback('x');
            std::cout << "New second character  : " << buffer->get() << '\n';
            std::cout << "Third character       : " << buffer->get() << '\n';
            buffer->unget();
            buffer->unget();
            std::cout << "Second character again: " << buffer->get() << '\n';
        }
    }

    void test_tokenizer()
    {
        std::string fib = R"(# Recursively calculate a Fibonacci number
var fib = function(n)
{
    if (n == 0 || n == 1)
        return n;

    return fib(n - 1) + fib(n - 2);
}

# Print the first ten Fibonacci numbers
for (i : 1..10)
    print("Fibbonacci for %{i} is %{fib(i)}\n");
)";

        std::unique_ptr<b::buffer> buffer = std::make_unique<b::string_buffer>(fib, "fib");
        l::tokenizer tokenizer(std::move(buffer));

//        std::cout << "Starting tokenization...\n";
//        for (;;)
//        {
//            auto& token = tokenizer.get();
////            if (token == compiler::tokens::end)
////                break;
//            if (token.is<t::end>())
//                break;
//
//
//        }

        std::cout << "Tokenization ended\n";
    }
}

int main()
{
    test_lexer_buffer();
    test_tokenizer();
    std::cout << std::flush;
}
