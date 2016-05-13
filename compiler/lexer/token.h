//
// Created by joachimp on 2016-04-26.
//

#ifndef RAVEN_LANG_TOKEN_H
#define RAVEN_LANG_TOKEN_H

namespace compiler
{
    namespace lexer
    {
        class token
        {
        public:
            std::string const& lexeme() const
            {
                return lexeme_;
            }

        private:
            std::string  filename_;    // Name of file token comes from
            unsigned int linenumber_;  // Line in file the token comes from
            std::string  lexeme_;      // The token lexeme
        };

        namespace tokens
        {
            class number : public token
            {
            public:
                int64_t value() const
                {
                    return value_;
                }
            private:
                int64_t value_;
            };

            class string : public token
            {
            public:
                std::string const& value() const
                {
                    return lexeme();
                }

            private:
            };

            class keyword : public token
            {
            public:
                std::string const& value() const
                {
                    return lexeme();
                }

            private:
            };

            class op : public token
            {
            public:

            private:
            };
        }
    }
}

#endif //RAVEN_LANG_TOKEN_H
