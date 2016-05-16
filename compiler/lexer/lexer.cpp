#include "lexer.h"

namespace compiler
{
    token& lexer::get()
    {
        static token dummy;
        return dummy;
    }
}

