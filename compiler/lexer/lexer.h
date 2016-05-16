#ifndef RAVEN_LANG_LEXER_H
#define RAVEN_LANG_LEXER_H

#include <queue>
#include <functional>
#include <istream>
#include "token.h"

namespace compiler
{
    class lexer
    {
    public:
        lexer()
            : inputs_{}, tokens_{}
        {}

        token& get();

    private:
        std::queue<std::reference_wrapper<std::istream>> inputs_;
        std::queue<token> tokens_;
    };
}

#endif //RAVEN_LANG_LEXER_H
