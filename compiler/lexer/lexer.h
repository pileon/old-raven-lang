#ifndef RAVEN_LANG_LEXER_H
#define RAVEN_LANG_LEXER_H

#include "../../autoconf.h"

#include <queue>
#include <deque>
#include <memory>

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

                // Check for comments
                if (ch == '#')
                {
                    skip_line();
                    continue;  // Continue to try next token
                }

                if (ch == '/')
                {
                    // TODO: Check the next character, if it's a '/' then we have a line
                    //       comment, if it's a '*' then we have a block comment
                    // TODO: skip_line() or skip_comment()
                }

                // TODO: Dummy token return
                return return_token(tokens::number, 0ll);
            }
        }
    }
}

#endif //RAVEN_LANG_LEXER_H
