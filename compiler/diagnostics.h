#ifndef RAVEN_LANG_DIAGNOSTICS_H
#define RAVEN_LANG_DIAGNOSTICS_H

#include <exception>
#include <string>

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
            basic_outputter& operator<<(std::basic_string<charT> const&)
            { return *this; }
            basic_outputter& operator<<(charT const)
            { return *this; }
            basic_outputter& operator<<(short const)
            { return *this; }
            basic_outputter& operator<<(int const)
            { return *this; }
            basic_outputter& operator<<(long const)
            { return *this; }
            basic_outputter& operator<<(long long const)
            { return *this; }
            basic_outputter& operator<<(unsigned short const)
            { return *this; }
            basic_outputter& operator<<(unsigned int const)
            { return *this; }
            basic_outputter& operator<<(unsigned long const)
            { return *this; }
            basic_outputter& operator<<(unsigned long long const)
            { return *this; }

            virtual void pre() = 0;
            virtual void post() = 0;
        };

        template<typename charT>
        struct info_outputter : basic_outputter<charT>
        {
            void pre()
            {
                // TODO: Output "info" plus file-name, line number, column
            }

            void post() {}
        };

    public:
        template<typename charT>
        static basic_outputter<charT>& info()
        {
            // TODO: Create info_outputter
            static info_outputter<charT> info;
            info.pre();
            return info;
        }
    };
}

#endif //RAVEN_LANG_DIAGNOSTICS_H
