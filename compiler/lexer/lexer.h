#ifndef RAVEN_LANG_LEXER_H
#define RAVEN_LANG_LEXER_H

#include "../../autoconf.h"

#include <queue>
#include <deque>
#include <memory>
#include <cstdint>
#include <cctype>
#include <functional>
#include <unordered_map>

#if defined(HAVE_HEADER_ANY)
# include <any>
#elif defined(HAVE_HEADER_EXPERIMENTAL_ANY)
# include <experimental/any>
// Pulling in symbols into the std namespace like this is not really
// allowed. It's done for forward compatibility with the C++17 standard
namespace std
{
    using std::experimental::any;
    using std::experimental::any_cast;
    using std::experimental::bad_any_cast;
}
#elif defined(HAVE_HEADER_BOOST_ANY)
# include <boost/any.hpp>
// Pulling in symbols into the std namespace like this is not really
// allowed. It's done for forward compatibility with the C++17 standard
namespace std
{
    using boost::any;
    using boost::any_cast;
    using boost::bad_any_cast;
}
#else
# error No any header available
#endif

#include "token.h"
#include "buffer.h"

namespace compiler
{
    namespace lexer
    {
        /**
         * \brief Lexical tokenizer class
         */
        template<typename charT>
        class basic_tokenizer
        {
        public:
            // Not default constructible
            basic_tokenizer() = delete;


            // Can only be created (explicitly) with a buffer
            explicit basic_tokenizer(std::unique_ptr<buffers::basic_buffer<charT>> buffer)
                : buffers_{}, linenumber_{1}
            {
                buffers_.push(std::move(buffer));
            }

            /**
             * \brief Get next token
             * \return The next foken from the buffer
             * \retval tokens::end No more tokens available
             */
            basic_token<charT> get();

            // Disallow copying
            basic_tokenizer(basic_tokenizer const&) = delete;
            basic_tokenizer& operator=(basic_tokenizer const&) = delete;

            basic_tokenizer(basic_tokenizer&&) = default;
            basic_tokenizer& operator=(basic_tokenizer&&) = default;

        private:
            std::queue<std::unique_ptr<buffers::basic_buffer<charT>>> buffers_;
            unsigned linenumber_;   // Current line number

            basic_token<charT> const return_token(tokens const token,
                                                  std::any const& data = std::any())
            {
                std::string const& name = buffers_.front()->name();
                return basic_token<charT>(name, linenumber_, token, data);
            }

            // Buffer convinience functions
            charT next()
            {
                return buffers_.front()->get();
            }

            void unget()
            {
                return buffers_.front()->unget();
            }

            charT peek() const
            {
                return buffers_.front()->peek();
            }

            bool is_end(charT const ch) const
            {
                return ch == buffers::basic_buffer<charT>::end;
            }

            void skip_line()
            {
                charT ch;
                while (!is_end(ch = next()) && ch != '\n')
                {
                }
            }

            bool isident(charT const ch) const
            {
                return (isalnum(ch) || ch == '_' || ch == '@' || ch == '$' || ch == '?');
            }

            void get_comment();

            basic_token<charT> get_number(int const base);
            basic_token<charT> get_string();
            basic_token<charT> get_identifier();
            basic_token<charT> get_operator();
        };

        using tokenizer = basic_tokenizer<char>;

        template<typename charT>
        basic_token<charT> basic_tokenizer<charT>::get()
        {
            charT ch;

            for (;;)
            {
                // First skip leading white-space, but not newlines
                while (!is_end(ch = next()) && ch != '\n' && std::isspace(ch))
                {
                }
                if (is_end(ch))
                    return return_token(tokens::end);

                // Check for newlines, used as statement separator
                if (ch == '\n')
                {
                    ++linenumber_;
                    return return_token(tokens::newline);
                }

                // Check for comments
                if (ch == '#')
                {
                    skip_line();
                    continue;  // Continue to try next token
                }

                if (ch == '/')
                {
                    charT nch = next();
                    if (nch == '/')
                    {
                        // C and C++ style line comments
                        skip_line();
                        continue;
                    }

                    if (nch == '*')
                    {
                        // C and C++ style block comments
                        // TODO: Handle it
                        continue;
                    }

                    unget();
                }

                // Check for numbers
                if (std::isdigit(ch))
                {
                    int base = 10;

                    if (ch == '0')
                    {
                        charT nch = next();
                        if (nch == 'x' || nch == 'X')
                            base = 16;
                        else if (nch == 'o' || nch == 'O')
                            base = 8;
                        else if (nch == 'b' || nch == 'B')
                            base = 2;
                        else if (isdigit(nch))
                        {
                            // C and C++ compatibility
                            unget();
                            base = 8;
                        }
                        else
                            unget();
                    }

                    if (base == 10)
                        unget();

                    return get_number(base);
                }

                // Check for strings
                if (ch == '"')
                {
                    return get_string();
                }

                // Check for keywords
                if (isident(ch))
                {
                    unget();
                    return get_identifier();
                }

                // Check for operators
                unget();
                return get_operator();
            }
        }

        template<typename charT>
        void basic_tokenizer<charT>::get_comment()
        {
            charT ch;

            while ((ch = next()) != buffers::basic_buffer<charT>::end)
            {
                if (ch == '\n')
                    ++linenumber_;
                else if (ch == '*')
                {
                    if (next() == '/')
                        break;
                    unget();
                }
                else if (ch == '/')
                {
                    if (next() == '*')
                        get_comment();
                    else
                        unget();
                }
            }
        }

        template<typename charT>
        basic_token<charT> basic_tokenizer<charT>::get_number(int const base)
        {
            std::function<int(int const)> isdigit;
            std::function<int(int const)> tonumber;

            auto dtonum = [](int const ch) -> int { return ch - '0'; };

            switch (base)
            {
            case 2:
                isdigit  = [](int const ch) -> int { return ch == '0' || ch == '1'; };
                tonumber = dtonum;
                break;
            case 8:
                isdigit  = [](int const ch) -> int { return std::isdigit(ch) && ch != '8' && ch != '9'; };
                tonumber = dtonum;
                break;
            case 10:
                isdigit  = std::bind(&::isdigit, std::placeholders::_1);
                tonumber = dtonum;
                break;
            case 16:
                isdigit  = std::bind(&::isxdigit, std::placeholders::_1);
                tonumber = [dtonum](int const ch) -> int { return (::isdigit(ch) ? dtonum(ch) : ::tolower(ch) - 'a'); };
                break;

            default:
                // TODO: Report and return error
                break;
            }

            std::int64_t value = 0;
            charT ch;

            while (isdigit(ch = next()))
            {
                value = value * base + tonumber(ch);
            }

            // Last character was not a digit, put it back
            unget();

            return return_token(tokens::number, value);
        }

        template<typename charT>
        basic_token<charT> basic_tokenizer<charT>::get_string()
        {
            charT ch;
            std::basic_string<charT> string;

            while ((ch = next()) != buffers::basic_buffer<charT>::end &&
                   ch != '"')
            {
                if (ch == '\\')
                {
                    ch = next();
                    switch (ch)
                    {
                    case 'n':
                        ch = '\n';
                        break;
                    case 'r':
                        ch = '\r';
                        break;
                    case 't':
                        ch = '\t';
                        break;
                    case 'a':
                        ch = '\a';
                        break;
                    case 'b':
                        ch = '\b';
                        break;
                    case '"':
                        ch = '\"';
                        break;
                    case '%':
                        ch = '%';
                        break;
                    case 'e':
                        ch = 0x1b;  // Escape
                        break;

                        // TODO: Handle octal and hexadecimal escape sequences

                    default:
                        // TODO: Report error
                        break;
                    }
                }

                string += ch;
            }

            if (ch == buffers::basic_buffer<charT>::end)
            {
                // TODO: Report and return error
            }

            return return_token(tokens::string, string);
        }

        template<typename charT>
        basic_token<charT> basic_tokenizer<charT>::get_identifier()
        {
            static std::unordered_map<std::string, tokens> const keywords = {
                { "var"     , tokens::k_var      },
                { "const"   , tokens::k_const    },
                { "return"  , tokens::k_return   },
                { "if"      , tokens::k_if       },
                { "else"    , tokens::k_else     },
                { "while"   , tokens::k_while    },
                { "do"      , tokens::k_do       },
                { "for"     , tokens::k_for      },
                { "break"   , tokens::k_break    },
                { "continue", tokens::k_continue },
                { "switch"  , tokens::k_switch   },
                { "case"    , tokens::k_case     },
                { "use"     , tokens::k_use      },
                { "function", tokens::k_function },
                { "class"   , tokens::k_class    },
                { "private" , tokens::k_private  },
                { "public"  , tokens::k_public   },
                { "object"  , tokens::k_object   },
            };

            charT ch;
            std::basic_string<charT> id;

            while (isident(ch = next()))
                id += ch;

            unget();

            auto keyword = keywords.find(id);
            if (keyword != keywords.end())
                return return_token(keyword->second, id);

            // TODO: Handle custom keywords

            return return_token(tokens::identifier, id);
        }

        template<typename charT>
        basic_token<charT> basic_tokenizer<charT>::get_operator()
        {
            charT ch = next();
            std::basic_string<charT> op{ ch };

            auto rt = [this, &op](tokens const t) -> basic_token<charT> {
                return return_token(t, op);
            };

            switch (ch)
            {
            case '<':
                ch = next();
                if (ch == '<')
                {
                    op += ch;
                    ch = next();
                    if (ch == '=')
                    {
                        op += ch;
                        return rt(tokens::assignment_bit_shift_left);
                    }
                    unget();
                    return rt(tokens::bit_shift_left);
                }
                unget();
                return rt(tokens::less_than);
            case '>':
                ch = next();
                if (ch == '>')
                {
                    op += ch;
                    ch = next();
                    if (ch == '=')
                    {
                        op += ch;
                        return rt(tokens::assignment_bit_shift_right);
                    }
                    unget();
                    return rt(tokens::bit_shift_right);
                }
                unget();
                return rt(tokens::greater_than);
            case '|':
                ch = next();
                if (ch == '|')
                {
                    op += ch;
                    return rt(tokens::logical_or);
                }
                else if (ch == '=')
                {
                    op += ch;
                    return rt(tokens::assignment_bit_or);
                }
                unget();
                return rt(tokens::pipe);
            case ',':
                return rt(tokens::comma);
            case ';':
                return rt(tokens::semicolon);
            case '.':
                ch = next();
                if (ch == '.')
                {
                    op += ch;
                    return rt(tokens::range);
                }
                unget();
                return rt(tokens::dot);
            case ':':
                return rt(tokens::colon);
            case '-':
                return rt(tokens::minus);
            case '!':
                ch = next();
                if (ch == '=')
                {
                    op += ch;
                    return rt(tokens::not_equal);
                }
                unget();
                return rt(tokens::logical_not);
            case '%':
                ch = next();
                if (ch == '=')
                {
                    op += ch;
                    return rt(tokens::assignment_modulo);
                }
                unget();
                return rt(tokens::percent);
            case '&':
                ch = next();
                if (ch == '=')
                {
                    op += ch;
                    return rt(tokens::assignment_modulo);
                }
                unget();
                return rt(tokens::percent);
            case '/':
                ch = next();
                if (ch == '=')
                {
                    op += ch;
                    return rt(tokens::assignment_divide);
                }
                unget();
                return rt(tokens::slash);
            case '{':
                return rt(tokens::left_curly_brace);
            case '(':
                return rt(tokens::left_parenthesis);
            case '[':
                return rt(tokens::left_square_brace);
            case ']':
                return rt(tokens::right_square_brace);
            case ')':
                return rt(tokens::right_parenthesis);
            case '}':
                return rt(tokens::right_curly_brace);
            case '=':
                ch = next();
                if (ch == '=')
                {
                    op += ch;
                    return rt(tokens::equal);
                }
                unget();
                return rt(tokens::assignment);
            case '+':
                ch = next();
                if (ch == '=')
                {
                    op += ch;
                    return rt(tokens::assignment_add);
                }
                unget();
                return rt(tokens::plus);
            case '^':
                ch = next();
                if (ch == '=')
                {
                    op += ch;
                    return rt(tokens::assignment_bit_xor);
                }
                unget();
                return rt(tokens::bit_xor);
            case '~':
                ch = next();
                if (ch == '=')
                {
                    op += ch;
                    return rt(tokens::assignment_bit_complement);
                }
                unget();
                return rt(tokens::tilde);
            case '*':
                ch = next();
                if (ch == '=')
                {
                    op += ch;
                    return rt(tokens::assignment_multiply);
                }
                else if (ch == '*')
                {
                    op += ch;
                    return rt(tokens::exponent);
                }
                unget();
                return rt(tokens::star);

            default:
                return rt(tokens::error);
            }
        }
    }
}

#endif //RAVEN_LANG_LEXER_H
