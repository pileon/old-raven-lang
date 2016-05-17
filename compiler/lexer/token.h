#ifndef RAVEN_LANG_TOKEN_H
#define RAVEN_LANG_TOKEN_H

namespace compiler
{
    template<typename charT>
    class basic_token
    {
    public:
        basic_token()
            : filename_{}, linenumber_{}, lexeme_{}
        {}

        std::basic_string<charT> const& lexeme() const
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
        template<typename charT>
        class basic_number : public basic_token<charT>
        {
        public:
            int64_t value() const
            {
                return value_;
            }
        private:
            int64_t value_;
        };

        template<typename charT>
        class basic_string : public basic_token<charT>
        {
        public:
            std::basic_string<charT> const& value() const
            {
                return this->lexeme();
            }

        private:
        };

        template<typename charT>
        class basic_keyword : public basic_token<charT>
        {
        public:
            std::basic_string<charT> const& value() const
            {
                return this->lexeme();
            }

        private:
        };

        template<typename charT>
        class basic_op : public basic_token<charT>
        {
        public:

        private:
        };
    }
}

#endif //RAVEN_LANG_TOKEN_H
