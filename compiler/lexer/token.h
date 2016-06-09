#ifndef RAVEN_LANG_TOKEN_H
#define RAVEN_LANG_TOKEN_H

#include "../../autoconf.h"

#include <string>
#if defined(HAVE_HEADER_ANY)
# include <any>
//#elif defined(HAVE_HEADER_EXPERIMENTAL_ANY)
//# include <experimental/any>
//// Pulling in symbols into the std namespace like this is not really
//// allowed. It's done for forward compatibility with the C++17 standard
//namespace std
//{
//    using std::experimental::any;
//    using std::experimental::any_cast;
//    using std::experimental::bad_any_cast;
//}
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

namespace compiler
{
    enum struct tokens : unsigned
    {
        // Literal tokens
        number,
        string,

        // Operator tokens
        assignment,                 //!< `=`
        assignment_add,             //!< `+=`
        assignment_subtract,        //!< `-=`
        assignment_multiply,        //!< `*=`
        assignment_divide,          //!< `/=`
        assignment_modulo,          //!< `%=`
        assignment_bit_complement,  //!< `~=`
        assignment_bit_or,          //!< `|=`
        assignment_bit_xor,         //!< `^=`
        assignment_bit_and,         //!< `&=`
        assignment_bit_shift_left,  //!< `<<=`
        assignment_bit_shift_right, //!< `>>=`
        left_parenthesis,           //!< `(`
        right_parenthesis,          //!< `)`
        left_square_brace,          //!< `[`
        right_square_brace,         //!< `]`
        left_curly_brace,           //!< `{`
        right_curly_brace,          //!< `}`
        dot,                        //!< `.`
        comma,                      //!< `,`
        colon,                      //!< `:`
        bit_xor,                    //!< `^`
        bit_and,                    //!< `&`
        bit_shift_left,             //!< `<<`
        bit_shift_right,            //!< `>>`
        logical_or,                 //!< `||`
        logical_and,                //!< `&&`
        logical_not,                //!< `!`
        equal,                      //!< `==`
        not_equal,                  //!< `!=`
        equal_less_than,            //!< `<=`
        equal_greater_than,         //!< `>=`
        less_than,                  //!< `<`
        greater_than,               //!< `>`
        approx_equal,               //!< `~=`
        approx_notequal,            //!< `!~=`
        plus,                       //!< `+`
        minus,                      //!< `-`
        star,                       //!< `*`
        percent,                    //!< `%`
        exponent,                   //!< `**`
        slash,                      //!< `/`
        pipe,                       //!< `|`
        tilde,                      //!< `~`
        range,                      //!< `..`

        // Keyword tokens
        // Note that these are prefixed with a `k_` because some keywords
        // are the same as C++ keywords. All keyword tokens are prefixed
        // for uniformity reasons.
        k_var,
        k_const,
        k_return,
        k_if,
        k_else,
        k_while,
        k_do,
        k_for,
        k_break,
        k_continue,
        k_switch,
        k_case,
        k_use,
        k_function,
        k_class,
        k_private,
        k_public,
        k_object,
        k_with,

        // Other tokens
        newline,        //!< Statement separator, ignored elsewhere
        semicolon,      //!< Statement separator
        identifier,     //!< Generic identifier
        end,            //!< End of the token-stream

        error = 999,    //!< A syntax error, and unrecognized token

        user_tokens = 1001  // Where user tokens begins
    };

    namespace lexer
    {
        // Forward declaration for friend declaration in token class
        template<typename charT>
        class basic_tokenizer;
    }

    template<typename charT>
    class basic_token
    {
        friend lexer::basic_tokenizer<charT>;

    public:
        basic_token() = delete;

        basic_token(basic_token const&) = default;
        basic_token(basic_token &&) = default;
        basic_token& operator=(basic_token const&) = default;
        basic_token& operator=(basic_token &&) = default;

        bool operator==(tokens const token)
        {
            return token_ == token;
        }

        bool operator==(basic_token const& token)
        {
            return *this == token.token_;
        }

        bool operator!=(tokens const token)
        {
            return !(*this == token);
        }

        bool operator!=(basic_token const& token)
        {
            return !(*this == token);
        }

        std::any const& data() const
        {
            return data_;
        }

        tokens token() const
        {
            return token_;
        }

        std::string const& name() const
        {
            return name_;
        }

        unsigned linenumber() const
        {
            return linenumber_;
        }

#if defined(TESTING)
    public:
#else
    private:
#endif
        explicit basic_token(std::string const& name, unsigned linenumber,
                             tokens const token, std::any const& data)
            : name_{name}, linenumber_{linenumber},
              token_{token}, data_{data}
        {}

        std::string  name_;     // Name of file token comes from
        unsigned linenumber_;   // Line in file the token comes from
        tokens token_;          // Token
        std::any data_;         // Token data
    };

    using token = basic_token<char>;
}

#endif //RAVEN_LANG_TOKEN_H
