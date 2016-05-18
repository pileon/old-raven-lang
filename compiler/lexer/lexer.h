#ifndef RAVEN_LANG_LEXER_H
#define RAVEN_LANG_LEXER_H

#include <queue>
#include <deque>
#include <memory>
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
                : buffers_{}, token_{}
            {
                buffers_.push(std::move(buffer));
            }

            /**
             * \brief Get next token
             * \return The next foken from the buffer
             * \retval tokens::end No more tokens available
             */
            basic_token<charT> const& get();

            // Disallow copying
            basic_tokenizer(basic_tokenizer const&) = delete;
            basic_tokenizer& operator=(basic_tokenizer const&) = delete;

            basic_tokenizer(basic_tokenizer&&) = default;
            basic_tokenizer& operator=(basic_tokenizer&&) = default;

        private:
            std::queue<std::unique_ptr<buffers::basic_buffer<charT>>> buffers_;
            std::unique_ptr<basic_token<charT>> token_;     //!< Last token returned from get()

            template<typename tokenT, typename... argsT>
            basic_token<charT> const& return_token(argsT&&... args)
            {
                token_ = std::make_unique<tokenT>(std::forward<argsT>(args)...);
                return *token_;
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
        basic_token<charT> const& basic_tokenizer<charT>::get()
        {
            charT ch;

            for (;;)
            {
                // First skip leading white-space, but not newlines
                while (!is_end(ch = next()) && ch != '\n' && std::isspace(ch))
                {
                }
                if (is_end(ch))
                    return return_token<tokens::end_<charT>>();

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
                return return_token<tokens::basic_number<charT>>(0);
            }
        }
    }
}

#endif //RAVEN_LANG_LEXER_H
