#include <cctype>
#include "lexer.h"

namespace compiler
{
    namespace lexer
    {
        template<typename charT>
        basic_token<charT> const& basic_tokenizer<charT>::get()
        {
            charT ch;

            // First skip leading white-space, but not newlines
            while (!is_end(ch = next_char()) && ch != '\n' && std::isspace(ch))
            {
            }
            if (is_end(ch))
                return_token<tokens::end_>();

            return_token<tokens::basic_number>(0);
        }
    }
}
