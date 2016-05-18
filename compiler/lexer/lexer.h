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

            explicit basic_tokenizer(std::unique_ptr<buffers::basic_buffer<charT>> buffer)
                : buffers_{}, token_{}
            {
                buffers_.push(buffer);
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
            basic_token<charT> const& return_token(argsT... args)
            {
                token_ = std::make_unique<tokenT>(std::forward(args...));
                return *token_;
            }

            charT next_char()
            {
                return buffers_.front().get();
            }
        };

        using tokenizer = basic_tokenizer<char>;
    }
}

#endif //RAVEN_LANG_LEXER_H
