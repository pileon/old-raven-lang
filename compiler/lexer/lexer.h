#ifndef RAVEN_LANG_LEXER_H
#define RAVEN_LANG_LEXER_H

#include "../../autoconf.h"

#include <queue>
#include <deque>
#include <memory>
#include <cstdint>
#include <cctype>
#include <cstdlib>
#include <functional>
#include <unordered_map>
#include <array>

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
        template<typename charO, typename charI>
        static charO T(charI const&);

        template<>
        constexpr char T<char, char>(char const& ch)
        {
            return ch;
        }

        template<>
        constexpr wchar_t T<wchar_t, wchar_t >(wchar_t const& ch)
        {
            return ch;
        }

//        template<>
//        wchar_t T<wchar_t, char>(char const& ch)
//        {
//            wchar_t out;
//            if (std::mbtowc(&out, &ch, 1) == -1)
//            {
//                throw "mbtowc error";
//                // TODO: Throw proper error
//            }
//            return out;
//        };

//        // TODO: Implement this
//        template<>
//        char T<char, wchar_t>(wchar_t const& ch);

        template<typename charO, typename charI>
        std::basic_string<charO> T(charI const*);

        template<>
        std::basic_string<char> T<char, char>(char const* s)
        {
            return s;
        }

        template<>
        std::basic_string<wchar_t> T<wchar_t, wchar_t>(wchar_t const* s)
        {
            return s;
        }

        // TODO: T string function for char -> wchar_t
        // TODO: T string function for wchar_t -> char

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
                while (!is_end(ch = next()) && ch != T<charT>('\n'))
                {
                }
            }

            static constexpr bool is_alpha(charT const& ch)
            {
                std::array<charT, 52> const letters = {{
                    T<charT>('a'), T<charT>('A'),
                    T<charT>('b'), T<charT>('B'),
                    T<charT>('c'), T<charT>('C'),
                    T<charT>('d'), T<charT>('D'),
                    T<charT>('e'), T<charT>('E'),
                    T<charT>('f'), T<charT>('F'),
                    T<charT>('g'), T<charT>('G'),
                    T<charT>('h'), T<charT>('H'),
                    T<charT>('i'), T<charT>('I'),
                    T<charT>('j'), T<charT>('J'),
                    T<charT>('k'), T<charT>('K'),
                    T<charT>('l'), T<charT>('L'),
                    T<charT>('m'), T<charT>('M'),
                    T<charT>('n'), T<charT>('N'),
                    T<charT>('o'), T<charT>('O'),
                    T<charT>('p'), T<charT>('P'),
                    T<charT>('q'), T<charT>('Q'),
                    T<charT>('r'), T<charT>('R'),
                    T<charT>('s'), T<charT>('S'),
                    T<charT>('t'), T<charT>('T'),
                    T<charT>('u'), T<charT>('U'),
                    T<charT>('v'), T<charT>('V'),
                    T<charT>('w'), T<charT>('W'),
                    T<charT>('x'), T<charT>('X'),
                    T<charT>('y'), T<charT>('Y'),
                    T<charT>('z'), T<charT>('Z')
                }};

                return std::find(letters.begin(), letters.end(), ch) != letters.end();
            }

            static constexpr bool is_digit(charT const& ch)
            {
                std::array<charT, 10> const digits = {{
                    T<charT>('0'),
                    T<charT>('1'),
                    T<charT>('2'),
                    T<charT>('3'),
                    T<charT>('4'),
                    T<charT>('5'),
                    T<charT>('6'),
                    T<charT>('7'),
                    T<charT>('8'),
                    T<charT>('9'),
                }};

                return std::find(digits.begin(), digits.end(), ch) != digits.end();
            }

            static constexpr bool is_xdigit(charT const& ch)
            {
                std::array<charT, 52> const letters = {{
                    T<charT>('a'), T<charT>('A'),
                    T<charT>('b'), T<charT>('B'),
                    T<charT>('c'), T<charT>('C'),
                    T<charT>('d'), T<charT>('D'),
                    T<charT>('e'), T<charT>('E'),
                    T<charT>('f'), T<charT>('F')
                }};

                return is_digit(ch) || std::find(letters.begin(), letters.end(), ch) != letters.end();
            }

            static constexpr bool is_alnum(charT const& ch)
            {
                return is_alpha(ch) || is_digit(ch);
            }

            static constexpr bool is_ident(charT const& ch)
            {
                return (is_alnum(ch) ||
                        ch == T<charT>('_') ||
                        ch == T<charT>('@') ||
                        ch == T<charT>('$') ||
                        ch == T<charT>('?'));
            }

            static constexpr bool is_space(charT const& ch)
            {
                std::array<charT, 5> const spaces = {{
                    T<charT>(' '),
                    T<charT>('\n'),
                    T<charT>('\r'),
                    T<charT>('\t'),
                    T<charT>('\v'),
                }};

                return std::find(spaces.begin(), spaces.end(), ch) != spaces.end();
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
                while (!is_end(ch = next()) && ch != T<charT>('\n') && is_space(ch))
                {
                }
                if (is_end(ch))
                    return return_token(tokens::end);

                // Check for newlines, used as statement separator
                if (ch == T<charT>('\n'))
                {
                    ++linenumber_;
                    return return_token(tokens::newline);
                }

                // Check for comments
                if (ch == T<charT>('#'))
                {
                    skip_line();
                    continue;  // Continue to try next token
                }

                if (ch == T<charT>('/'))
                {
                    charT nch = next();
                    if (nch == T<charT>('/'))
                    {
                        // C and C++ style line comments
                        skip_line();
                        continue;
                    }

                    if (nch == T<charT>('*'))
                    {
                        // C and C++ style block comments
                        get_comment();
                        continue;
                    }

                    unget();
                }

                // Check for numbers
                if (is_digit(ch))
                {
                    int base = 10;

                    if (ch == T<charT>('0'))
                    {
                        charT nch = next();
                        if (nch == T<charT>('x') || nch == T<charT>('X'))
                            base = 16;
                        else if (nch == T<charT>('o') || nch == T<charT>('O'))
                            base = 8;
                        else if (nch == T<charT>('b') || nch == T<charT>('B'))
                            base = 2;
                        else if (is_digit(nch))
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
                if (ch == T<charT>('"'))
                {
                    return get_string();
                }

                // Check for keywords
                if (is_ident(ch))
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
                if (ch == T<charT>('\n'))
                    ++linenumber_;
                else if (ch == T<charT>('*'))
                {
                    if (next() == T<charT>('/'))
                        break;
                    unget();
                }
                else if (ch == T<charT>('/'))
                {
                    if (next() == T<charT>('*'))
                        get_comment();
                    else
                        unget();
                }
            }
        }

        template<typename charT>
        basic_token<charT> basic_tokenizer<charT>::get_number(int const base)
        {
            std::function<bool(charT const&)> isdigit;
            std::function<int(charT const&)> tonumber;

            auto dtonum = [](charT const& ch) -> int { return ch - T<charT>('0'); };

            switch (base)
            {
            case 2:
                isdigit  = [](charT const& ch) -> bool { return ch == T<charT>('0') || ch == T<charT>('1'); };
                tonumber = dtonum;
                break;
            case 8:
                isdigit  = [](charT const& ch) -> bool { return std::isdigit(ch) && ch != T<charT>('8') && ch != T<charT>('9'); };
                tonumber = dtonum;
                break;
            case 10:
                isdigit  = std::bind(is_digit, std::placeholders::_1);
                tonumber = dtonum;
                break;
            case 16:
                isdigit  = std::bind(is_xdigit, std::placeholders::_1);
                tonumber = [dtonum](charT const& ch) -> int { return (is_digit(ch) ? dtonum(ch) : ::tolower(ch) - T<charT>('a')); };
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
                   ch != T<charT>('"'))
            {
                if (ch == T<charT>('\\'))
                {
                    ch = next();
                    switch (ch)
                    {
                    case T<charT>('n'):
                        ch = T<charT>('\n');
                        break;
                    case T<charT>('r'):
                        ch = T<charT>('\r');
                        break;
                    case T<charT>('t'):
                        ch = T<charT>('\t');
                        break;
                    case T<charT>('a'):
                        ch = T<charT>('\a');
                        break;
                    case T<charT>('b'):
                        ch = T<charT>('\b');
                        break;
                    case T<charT>('"'):
                        ch = T<charT>('\"');
                        break;
                    case T<charT>('%'):
                        ch = T<charT>('%');
                        break;
                    case T<charT>('e'):
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
            static std::unordered_map<std::basic_string<charT>, tokens> keywords;

            if (keywords.empty())
            {
                keywords.emplace(T<charT>("var")     , tokens::k_var      );
                keywords.emplace(T<charT>("const")   , tokens::k_const    );
                keywords.emplace(T<charT>("return")  , tokens::k_return   );
                keywords.emplace(T<charT>("if")      , tokens::k_if       );
                keywords.emplace(T<charT>("else")    , tokens::k_else     );
                keywords.emplace(T<charT>("while")   , tokens::k_while    );
                keywords.emplace(T<charT>("do")      , tokens::k_do       );
                keywords.emplace(T<charT>("for")     , tokens::k_for      );
                keywords.emplace(T<charT>("break")   , tokens::k_break    );
                keywords.emplace(T<charT>("continue"), tokens::k_continue );
                keywords.emplace(T<charT>("switch")  , tokens::k_switch   );
                keywords.emplace(T<charT>("case")    , tokens::k_case     );
                keywords.emplace(T<charT>("use")     , tokens::k_use      );
                keywords.emplace(T<charT>("function"), tokens::k_function );
                keywords.emplace(T<charT>("class")   , tokens::k_class    );
                keywords.emplace(T<charT>("private") , tokens::k_private  );
                keywords.emplace(T<charT>("public")  , tokens::k_public   );
                keywords.emplace(T<charT>("object")  , tokens::k_object   );
            }

            charT ch;
            std::basic_string<charT> id;

            while (is_ident(ch = next()))
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
            case T<charT>('<'):
                ch = next();
                if (ch == T<charT>('<'))
                {
                    op += ch;
                    ch = next();
                    if (ch == T<charT>('='))
                    {
                        op += ch;
                        return rt(tokens::assignment_bit_shift_left);
                    }
                    unget();
                    return rt(tokens::bit_shift_left);
                }
                unget();
                return rt(tokens::less_than);
            case T<charT>('>'):
                ch = next();
                if (ch == T<charT>('>'))
                {
                    op += ch;
                    ch = next();
                    if (ch == T<charT>('='))
                    {
                        op += ch;
                        return rt(tokens::assignment_bit_shift_right);
                    }
                    unget();
                    return rt(tokens::bit_shift_right);
                }
                unget();
                return rt(tokens::greater_than);
            case T<charT>('|'):
                ch = next();
                if (ch == T<charT>('|'))
                {
                    op += ch;
                    return rt(tokens::logical_or);
                }
                else if (ch == T<charT>('='))
                {
                    op += ch;
                    return rt(tokens::assignment_bit_or);
                }
                unget();
                return rt(tokens::pipe);
            case T<charT>(','):
                return rt(tokens::comma);
            case T<charT>(';'):
                return rt(tokens::semicolon);
            case T<charT>('.'):
                ch = next();
                if (ch == T<charT>('.'))
                {
                    op += ch;
                    return rt(tokens::range);
                }
                unget();
                return rt(tokens::dot);
            case T<charT>(':'):
                return rt(tokens::colon);
            case T<charT>('-'):
                return rt(tokens::minus);
            case T<charT>('!'):
                ch = next();
                if (ch == T<charT>('='))
                {
                    op += ch;
                    return rt(tokens::not_equal);
                }
                unget();
                return rt(tokens::logical_not);
            case T<charT>('%'):
                ch = next();
                if (ch == T<charT>('='))
                {
                    op += ch;
                    return rt(tokens::assignment_modulo);
                }
                unget();
                return rt(tokens::percent);
            case T<charT>('&'):
                ch = next();
                if (ch == T<charT>('='))
                {
                    op += ch;
                    return rt(tokens::assignment_modulo);
                }
                unget();
                return rt(tokens::percent);
            case T<charT>('/'):
                ch = next();
                if (ch == T<charT>('='))
                {
                    op += ch;
                    return rt(tokens::assignment_divide);
                }
                unget();
                return rt(tokens::slash);
            case T<charT>('{'):
                return rt(tokens::left_curly_brace);
            case T<charT>('('):
                return rt(tokens::left_parenthesis);
            case T<charT>('['):
                return rt(tokens::left_square_brace);
            case T<charT>(']'):
                return rt(tokens::right_square_brace);
            case T<charT>(')'):
                return rt(tokens::right_parenthesis);
            case T<charT>('}'):
                return rt(tokens::right_curly_brace);
            case T<charT>('='):
                ch = next();
                if (ch == T<charT>('='))
                {
                    op += ch;
                    return rt(tokens::equal);
                }
                unget();
                return rt(tokens::assignment);
            case T<charT>('+'):
                ch = next();
                if (ch == T<charT>('='))
                {
                    op += ch;
                    return rt(tokens::assignment_add);
                }
                unget();
                return rt(tokens::plus);
            case T<charT>('^'):
                ch = next();
                if (ch == T<charT>('='))
                {
                    op += ch;
                    return rt(tokens::assignment_bit_xor);
                }
                unget();
                return rt(tokens::bit_xor);
            case T<charT>('~'):
                ch = next();
                if (ch == T<charT>('='))
                {
                    op += ch;
                    return rt(tokens::assignment_bit_complement);
                }
                unget();
                return rt(tokens::tilde);
            case T<charT>('*'):
                ch = next();
                if (ch == T<charT>('='))
                {
                    op += ch;
                    return rt(tokens::assignment_multiply);
                }
                else if (ch == T<charT>('*'))
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
