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
            basic_tokenizer()
                : buffers_{}, token_{}
            {}

            /**
             * \brief Get next token
             * \return The next foken from the buffer
             * \retval tokens::end No more tokens available
             */
            basic_token<charT> const& get();

        private:
            std::queue<std::unique_ptr<buffers::basic_buffer<charT>>> buffers_;
            basic_token<charT> token_;     //!< Last token returned from get()
        };

        using tokenizer = basic_tokenizer<char>;
    }
}

#endif //RAVEN_LANG_LEXER_H
