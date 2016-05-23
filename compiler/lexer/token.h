#ifndef RAVEN_LANG_TOKEN_H
#define RAVEN_LANG_TOKEN_H

#include "../../autoconf.h"

#include <string>
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

namespace compiler
{
    // TODO: Turn into enumeration
    namespace tokens
    {
        // Literal tokens
        static constexpr unsigned number = 301;
        static constexpr unsigned string = 302;

        // Operator tokens
        static constexpr unsigned assignment                    = 311;  //!< `=`
        static constexpr unsigned assignment_add                = 312;  //!< `+=`
        static constexpr unsigned assignment_subtract           = 313;  //!< `-=`
        static constexpr unsigned assignment_multiply           = 314;  //!< `*=`
        static constexpr unsigned assignment_divide             = 315;  //!< `/=`
        static constexpr unsigned assignment_modulo             = 316;  //!< `%=`
        static constexpr unsigned assignment_bit_or             = 317;  //!< `|=`
        static constexpr unsigned assignment_bit_and            = 318;  //!< `&=`
        static constexpr unsigned assignment_bit_shift_left     = 319;  //!< `<<=`
        static constexpr unsigned assignment_bit_shift_right    = 320;  //!< `>>=`
        static constexpr unsigned left_parenthesis              = 321;  //!< `(`
        static constexpr unsigned right_parenthesis             = 322;  //!< `)`
        static constexpr unsigned left_square_brace             = 323;  //!< `[`
        static constexpr unsigned right_square_brace            = 324;  //!< `]`
        static constexpr unsigned left_curly_brace              = 325;  //!< `{`
        static constexpr unsigned right_curly_brace             = 326;  //!< `}`
        static constexpr unsigned dot                           = 327;  //!< `.`
        static constexpr unsigned comma                         = 328;  //!< `,`
        static constexpr unsigned colon                         = 329;  //!< `:`
        static constexpr unsigned bit_xor                       = 321;  //!< `^`
        static constexpr unsigned bit_and                       = 322;  //!< `&`
        static constexpr unsigned bit_shift_left                = 324;  //!< `<<`
        static constexpr unsigned bit_shift_right               = 325;  //!< `>>`
        static constexpr unsigned logical_or                    = 326;  //!< `||`
        static constexpr unsigned logical_and                   = 327;  //!< `&&`
        static constexpr unsigned logical_not                   = 328;  //!< `!`
        static constexpr unsigned equal                         = 329;  //!< `==`
        static constexpr unsigned not_equal                     = 330;  //!< `!=`
        static constexpr unsigned equal_less_than               = 331;  //!< `<=`
        static constexpr unsigned equal_greater_than            = 332;  //!< `>=`
        static constexpr unsigned less_than                     = 333;  //!< `<`
        static constexpr unsigned greater_than                  = 334;  //!< `>`
        static constexpr unsigned approx_equal                  = 335;  //!< `~=`
        static constexpr unsigned approx_notequal               = 336;  //!< `!~=`
        static constexpr unsigned plus                          = 337;  //!< `+`
        static constexpr unsigned minus                         = 338;  //!< `-`
        static constexpr unsigned star                          = 339;  //!< `*`
        static constexpr unsigned percent                       = 340;  //!< `%`
        static constexpr unsigned exponent                      = 341;  //!< `**`
        static constexpr unsigned slash                         = 342;  //!< `/`
        static constexpr unsigned pipe                          = 343;  //!< `|`
        static constexpr unsigned tilde                         = 344;  //!< `~`
        static constexpr unsigned range                         = 344;  //!< `...`
        static constexpr unsigned assignment_bit_xor            = 345;  //!< `^=`

        // Keyword tokens
        // Note that these are prefixed with a `k_` because some keywords
        // are the same as C++ keywords. All keyword tokens are prefixed
        // for uniformity reasons.
        static constexpr unsigned k_var         = 361;
        static constexpr unsigned k_const       = 362;
        static constexpr unsigned k_return      = 363;
        static constexpr unsigned k_if          = 364;
        static constexpr unsigned k_else        = 365;
        static constexpr unsigned k_while       = 366;
        static constexpr unsigned k_do          = 367;
        static constexpr unsigned k_for         = 368;
        static constexpr unsigned k_break       = 369;
        static constexpr unsigned k_continue    = 370;
        static constexpr unsigned k_switch      = 371;
        static constexpr unsigned k_case        = 372;
        static constexpr unsigned k_use         = 373;
        static constexpr unsigned k_function    = 374;
        static constexpr unsigned k_class       = 375;
        static constexpr unsigned k_private     = 376;
        static constexpr unsigned k_public      = 377;
        static constexpr unsigned k_object      = 378;

        // Other tokens
        static constexpr unsigned newline   = 391;      //!< Statement separator, ignored elsewhere
        static constexpr unsigned semicolon = 392;      //!< Statement separator
        static constexpr unsigned end       = 393;      //!< End of the token-stream

        static constexpr  unsigned user_tokens = 700;  // Where user tokens begins
    }

    template<typename charT>
    class basic_token
    {
    public:
        ~basic_token()
        {}

        // Tokens can not be copied or moved
        basic_token(basic_token const&) = delete;
        basic_token(basic_token &&) = delete;
        basic_token& operator=(basic_token const&) = delete;
        basic_token& operator=(basic_token &&) = delete;

        std::basic_string<charT> const& lexeme() const
        {
            return lexeme_;
        }

    private:
        std::string  filename_;    // Name of file token comes from
        unsigned int linenumber_;  // Line in file the token comes from
        std::basic_string<charT>  lexeme_;      // The token lexeme
        std::any value;
    };

    using token = basic_token<char>;
}

#endif //RAVEN_LANG_TOKEN_H
