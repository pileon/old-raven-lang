//
// Created by arrow on 2016-06-06.
//

#include "diagnostics.h"

int main()
{
    compiler::token tok("bar", 1, compiler::tokens::error, 0);
    compiler::diagnostics::info() << tok << "foo";
}
