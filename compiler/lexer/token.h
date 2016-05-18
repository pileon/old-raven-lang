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

        /**
         * \brief Compare to other token
         * \return If the two tokens are equal or not
         * \retval true The two tokens are equal
         * \retval false The two tokens are not equal
         */
        friend bool operator==(basic_token const& lhs, basic_token const& rhs)
        {
            return lhs.is_equal_to(rhs);
        }

        friend bool operator!=(basic_token const& lhs, basic_token const& rhs)
        {
            return !(lhs == rhs);
        }

    protected:
        virtual bool is_equal_to(basic_token const&) const = 0;

        void lexeme(std::basic_string<charT> const& lex)
        {
            lexeme_ = lex;
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
            basic_number() = delete;

            explicit basic_number(int64_t value)
                : basic_token<charT>{}, value_{value}
            {}

            int64_t value() const
            {
                return value_;
            }

        protected:
            bool is_equal_to(basic_token<charT> const&) const
            {
                return false;
            }

        private:
            int64_t value_;
        };

        template<typename charT>
        class basic_string : public basic_token<charT>
        {
        public:
            basic_string() = delete;

            explicit basic_string(std::basic_string<charT> const& value)
                 : basic_token<charT>{}
            {
                lexeme(value);
            }

            std::basic_string<charT> const& value() const
            {
                return this->lexeme();
            }

        protected:
            bool is_equal_to(basic_token<charT> const&) const
            {
                return false;
            }

        private:
        };

        template<typename charT>
        class basic_keyword : public basic_token<charT>
        {
        public:
            basic_keyword() = delete;

            explicit basic_keyword(std::basic_string<charT> const& value)
                : basic_token<charT>{}
            {
                lexeme(value);
            }

            std::basic_string<charT> const& value() const
            {
                return this->lexeme();
            }

        protected:
            bool is_equal_to(basic_token<charT> const&) const
            {
                return false;
            }

        private:
        };

        template<typename charT>
        class basic_op : public basic_token<charT>
        {
        public:
            basic_op() = delete;

            explicit basic_op(std::basic_string<charT> const& value)
                : basic_token<charT>{}
            {
                lexeme(value);
            }

        protected:
            bool is_equal_to(basic_token<charT> const&) const
            {
                return false;
            }

        private:
        };

        template<typename charT>
        struct end_ : public basic_token<charT>
        {
            end_() : basic_token<charT>{ }
            {
            }

        protected:
            bool is_equal_to(basic_token<charT> const&) const
            {
                return false;
            }
        };

        static end_<char> end;
    }
}

#endif //RAVEN_LANG_TOKEN_H
