//
// Created by joachimp on 2016-05-16.
//

#include <iostream>
#include <memory>
#include "buffer.h"

namespace b = compiler::lexer::buffers;

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
}

int main()
{
    test_lexer_buffer();
}
