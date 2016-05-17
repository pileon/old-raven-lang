#ifndef RAVEN_LANG_LEXER_H
#define RAVEN_LANG_LEXER_H

#include <queue>
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
                : buffers_{}
            {}

        private:
            std::queue<std::unique_ptr<buffers::basic_buffer<charT>>> buffers_;
        };

        using tokenizer = basic_tokenizer<char>;
    }
//    class lexer
//    {
//    public:
//        lexer()
//            : inputs_{}, tokens_{}
//        {}
//
//        token& get();
//
//    private:
//        std::queue<std::reference_wrapper<std::istream>> inputs_;
//        std::queue<token> tokens_;
//    };
}

#endif //RAVEN_LANG_LEXER_H
