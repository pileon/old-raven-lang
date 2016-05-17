#ifndef RAVEN_LANG_TOKEN_H
#define RAVEN_LANG_TOKEN_H

#include <string>

namespace compiler
{
    template<typename charT>
    class basic_token
    {
    public:
        basic_token()
            : filename_{}, linenumber_{}, lexeme_{}
        {}

        virtual ~basic_token()
        {}

        // Tokens can not be copied or moved
        basic_token(basic_token const&) = delete;
        basic_token(basic_token &&) = delete;
        basic_token& operator=(basic_token const&) = delete;
        basic_token& operator=(basic_token &&) = delete;

        std::basic_string<charT> const& lexeme() const
        {
            return lexeme_;
        }

    private:
        std::string  filename_;    // Name of file token comes from
        unsigned int linenumber_;  // Line in file the token comes from
        std::basic_string<charT>  lexeme_;      // The token lexeme
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

        template<typename charT>
        struct end_ : public basic_token<charT>
        {
            end_() : basic_token<charT>{ }
            {
            }
        };

        static end_<char> end;

        template<typename charT>
        bool operator==(basic_token<charT> const& lhs, end_<charT> const& rhs)
        {
            return false;
        }

        template<typename charT>
        bool operator!=(end_<charT> const& lhs, basic_token<charT> const& rhs)
        {
            return false;
        }
    }

    /**
     * \brief Compare to other token
     * \return If the two tokens are equal or not
     * \retval true The two tokens are equal
     * \retval false The two tokens are not equal
     */
    template<typename charT>
    bool operator==(basic_token<charT> const& lhs, basic_token<charT> const& rhs)
    {
        if (&lhs == &rhs)
            return true;

        return lhs.lexeme() == rhs.lexeme();
    }

    template<typename charT>
    bool operator!=(basic_token<charT> const& lhs, basic_token<charT> const& rhs)
    {
        return !(lhs == rhs);
    }
}

#endif //RAVEN_LANG_TOKEN_H
