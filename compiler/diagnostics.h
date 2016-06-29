#ifndef RAVEN_LANG_DIAGNOSTICS_H
#define RAVEN_LANG_DIAGNOSTICS_H

#include <exception>
#include <string>
#include <iostream>

#include "lexer/token.h"

namespace compiler
{
    /// \brief Diagnostic message handling
    ///
    class diagnostics
    {
    private:
        template<typename charT>
        struct basic_outputter
        {
            basic_outputter& operator<<(basic_token<charT> const& token)
            {
                output_ << token.name() << ':' << token.linenumber() << ": ";
                return *this;
            }

            basic_outputter& operator<<(std::basic_string<charT> const& value)
            {
                output_ << value;
                return *this;
            }
            basic_outputter& operator<<(charT const value)
            {
                output_ << value;
                return *this;
            }
            basic_outputter& operator<<(short const value)
            {
                output_ << value;
                return *this;
            }
            basic_outputter& operator<<(int const value)
            {
                output_ << value;
                return *this;
            }
            basic_outputter& operator<<(long const value)
            {
                output_ << value;
                return *this;
            }
            basic_outputter& operator<<(long long const value)
            {
                output_ << value;
                return *this;
            }
            basic_outputter& operator<<(unsigned short const value)
            {
                output_ << value;
                return *this;
            }
            basic_outputter& operator<<(unsigned int const value)
            {
                output_ << value;
                return *this;
            }
            basic_outputter& operator<<(unsigned long const value)
            {
                output_ << value;
                return *this;
            }
            basic_outputter& operator<<(unsigned long long const value)
            {
                output_ << value;
                return *this;
            }

            virtual basic_outputter& pre() = 0;
            virtual basic_outputter& post() = 0;

            basic_outputter() = delete;
            basic_outputter(basic_outputter const&) = default;
            basic_outputter(basic_outputter&&) = default;

            basic_outputter(std::basic_ostream<charT>& os)
                : output_(os)
            {}

            virtual ~basic_outputter() {}

            std::basic_ostream<charT>& output_;
        };
        // TODO: Specialization of basic_outputter for handling char and wchar_t respectively?
        // TODO: Then the class can handle both char and wchar_t overloads of the << operator,
        // TODO: converting as needed

        template<typename charT>
        struct info_outputter : basic_outputter<charT>
        {
            info_outputter(std::basic_ostream<charT>& os)
                : basic_outputter<charT>(os)
            {}

            basic_outputter<charT>& pre()
            {
                return *this << "info: ";
                // TODO: Output "info" plus file-name, line number, column
                // TODO: How to get file-name, line number or column?
                return *this;
            }

            basic_outputter<charT>& post()
            {
                return *this;
            }
        };

    public:
        /// \brief To output informational messages
        /// \return An object that outputs informational messages
        template<typename charT>
        static basic_outputter<charT> basic_info()
        {
            return info_outputter<charT>{std::clog}.pre();
        }

        static constexpr auto info = basic_info<char>;

    private:
    };
}

#endif //RAVEN_LANG_DIAGNOSTICS_H
