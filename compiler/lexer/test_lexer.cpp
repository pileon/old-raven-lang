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

    void tokenize(std::string const& input, std::string const& name)
    {
        std::unique_ptr<b::buffer> buffer = std::make_unique<b::string_buffer>(input, name);
        l::tokenizer tokenizer(std::move(buffer));

        std::cout << "Starting tokenization...\n";
        for (;;)
        {
            auto token = tokenizer.get();
            if (token == compiler::tokens::end)
                break;

            if (token == compiler::tokens::newline)
                continue;

            std::cout << token.name() << ':' << token.linenumber() << ':';

            switch (token.token())
            {
            case compiler::tokens::number:
                std::cout << " Number: " << std::any_cast<std::int64_t>(token.data()) << '\n';
                break;

            case compiler::tokens::identifier:
                std::cout << " Identifier: " << std::any_cast<std::string>(token.data()) << '\n';
                break;

            case compiler::tokens::string:
                std::cout << " String: \"" << std::any_cast<std::string>(token.data()) << "\"\n";
                break;

            case compiler::tokens::k_var:
            case compiler::tokens::k_const:
            case compiler::tokens::k_return:
            case compiler::tokens::k_if:
            case compiler::tokens::k_else:
            case compiler::tokens::k_while:
            case compiler::tokens::k_do:
            case compiler::tokens::k_for:
            case compiler::tokens::k_break:
            case compiler::tokens::k_continue:
            case compiler::tokens::k_switch:
            case compiler::tokens::k_case:
            case compiler::tokens::k_use:
            case compiler::tokens::k_function:
            case compiler::tokens::k_class:
            case compiler::tokens::k_private:
            case compiler::tokens::k_public:
            case compiler::tokens::k_object:
                std::cout << " Keyword: " << std::any_cast<std::string>(token.data()) << '\n';
                break;

            case compiler::tokens::assignment:
            case compiler::tokens::assignment_add:
            case compiler::tokens::assignment_subtract:
            case compiler::tokens::assignment_multiply:
            case compiler::tokens::assignment_divide:
            case compiler::tokens::assignment_modulo:
            case compiler::tokens::assignment_bit_complement:
            case compiler::tokens::assignment_bit_or:
            case compiler::tokens::assignment_bit_xor:
            case compiler::tokens::assignment_bit_and:
            case compiler::tokens::assignment_bit_shift_left:
            case compiler::tokens::assignment_bit_shift_right:
            case compiler::tokens::left_parenthesis:
            case compiler::tokens::right_parenthesis:
            case compiler::tokens::left_square_brace:
            case compiler::tokens::right_square_brace:
            case compiler::tokens::left_curly_brace:
            case compiler::tokens::right_curly_brace:
            case compiler::tokens::dot:
            case compiler::tokens::comma:
            case compiler::tokens::colon:
            case compiler::tokens::bit_xor:
            case compiler::tokens::bit_and:
            case compiler::tokens::bit_shift_left:
            case compiler::tokens::bit_shift_right:
            case compiler::tokens::logical_or:
            case compiler::tokens::logical_and:
            case compiler::tokens::logical_not:
            case compiler::tokens::equal:
            case compiler::tokens::not_equal:
            case compiler::tokens::equal_less_than:
            case compiler::tokens::equal_greater_than:
            case compiler::tokens::less_than:
            case compiler::tokens::greater_than:
            case compiler::tokens::approx_equal:
            case compiler::tokens::approx_notequal:
            case compiler::tokens::plus:
            case compiler::tokens::minus:
            case compiler::tokens::star:
            case compiler::tokens::percent:
            case compiler::tokens::exponent:
            case compiler::tokens::slash:
            case compiler::tokens::pipe:
            case compiler::tokens::tilde:
            case compiler::tokens::range:
                std::cout << " Operator: " <<  std::any_cast<std::string>(token.data()) << '\n';
                break;

            case compiler::tokens::newline:
                break;

            case compiler::tokens::semicolon:
                std::cout << " ;\n";
                break;

            case compiler::tokens::error:
                std::cout << " Error: \"" <<  std::any_cast<std::string>(token.data()) << "\"\n";
                break;

            default:
                std::cout << " Unknown token: #" << static_cast<unsigned>(token.token()) << '\n';
                break;
            }
        }

        std::cout << "Tokenization ended\n";
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

//        tokenize(fib, "fib");

        tokenize("123 0b101 0123 0x123", "numbers");
    }
}

int main()
{
    test_lexer_buffer();
    test_tokenizer();
    std::cout << std::flush;
}
