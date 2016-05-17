#ifndef RAVEN_LANG_BUFFER_H
#define RAVEN_LANG_BUFFER_H

#include <string>

namespace compiler
{
    namespace lexer
    {
        namespace buffers
        {
            /**
             * \brief Base class to handle buffering of input for the lexer
             * \param charT The character type for the buffering
             *
             * The buffer scheme uses two buffers, one "current" buffer and one that was
             * the previous buffer. When the end of the current buffer is reached the
             * previous buffer is filled with data, and then et to the current buffer.
             *
             * Using double-buffering like this gives us large headroom for unget/putback,
             * which helps the lexer handle advanced multi-character tokens.
             */
            template<typename charT> class basic_buffer
            {
            public:
                virtual ~basic_buffer()
                {
                }

                /**
                 * \brief Get the name of the buffer
                 * \return A string containing the name of the buffer
                 */
                std::string name() const
                {
                    return name_;
                }

                /**
                 * \brief Get the next character from the current buffer
                 * \return The next character
                 * \retval end Returned when there is no more characters to read
                 * \note Made virtual because it's overridden by basic_string_buffer
                 */
                virtual charT get()
                {
                    return end;
                }

                /**
                 * \brief "Unget" the last character returned by get()
                 * \note Made virtual because it's overridden by basic_string_buffer
                 */
                virtual void unget()
                {
                }

                /**
                 * \brief Put back a character into the buffer
                 * \param ch The character to put back
                 * \note Made virtual because it's overridden by basic_string_buffer
                 *
                 * Put back a character into the buffer. This will be the next character
                 * returned by get().
                 */
                virtual void putback(charT const& ch __attribute__((unused)))
                {
                }

                /**
                 * \brief Returned by get() to mark the end of the input
                 */
                enum : charT
                {
                    end = -1
                };

                // Disallow copying
                basic_buffer(basic_buffer const&) = delete;
                basic_buffer& operator=(basic_buffer const&) = delete;

                basic_buffer(basic_buffer&&) = default;
                basic_buffer& operator=(basic_buffer&&) = default;

            protected:
                basic_buffer()
                        : name_{ }, size_{ }, buffer_{ }, endfirst_{ }, endsecond_{ }, end_{ }, current_{ }
                {
                }

                basic_buffer(std::string const& name)
                        : name_{ name }, size_{ }, buffer_{ }, endfirst_{ }, endsecond_{ }, end_{ }, current_{ }
                {
                }

                /**
                 * \brief Set the name of the buffer
                 * \param name New name of the buffer
                 */
                void name(std::string const& name)
                {
                    name_ = name;
                }

                /**
                 * \brief Fill up a buffer with new data
                 * \param buffer The buffer to fill
                 * \param length The number of characters to read into the buffer
                 * \return The number of characters read into the buffer
                 * \retval 0 When the end has been reached
                 */
                virtual size_t fill_buffer(charT *buffer, size_t const length) = 0;

            private:
                std::string name_;  //!< The name of the buffer
                size_t      size_;       //!< The size of a single buffer
                charT *buffer_;     //!< The double buffer
                charT *endfirst_;   //!< The end of the first buffer
                charT *endsecond_;  //!< The end of the second buffer
                charT *end_;        //!< If not nullptr, marks the final end of the input
                charT *current_;    //!< Current position in the buffer(s)
            };

            using buffer = basic_buffer<char>;


            /**
             * \brief A string buffer
             *
             * Use a string as a buffer. Will return `end` when at the end of the string.
             */
            template<typename charT> class basic_string_buffer
                    : public basic_buffer<charT>
            {
            public:
                using basic_buffer<charT>::end;

                basic_string_buffer()
                        : basic_buffer<charT>(), string_{ }, current_{ string_.end() }
                {
                }

                basic_string_buffer(std::basic_string<charT> const& string, std::string const& name = "")
                        : basic_buffer<charT>(name), string_{ string }, current_{ string_.begin() }
                {
                }

                void set(std::basic_string<charT> const& string)
                {
                    string_  = string;
                    current_ = string_.begin();
                }

                charT get()
                {
                    if (current_ == string_.end())
                        return end;
                    return *current_++;
                }

                void unget()
                {
                    if (current_ != string_.begin())
                        --current_;
                }

                void putback(charT const& ch)
                {
                    if (current_ != string_.begin())
                    {
                        unget();
                        *current_ = ch;
                    }
                }

            private:
                using string_t = std::basic_string<charT>;

                size_t fill_buffer(charT *, size_t const)
                {
                    return 0;
                }

                string_t                    string_;    //!< The string buffer
                typename string_t::iterator current_; //!< Current character
            };

            using string_buffer = basic_string_buffer<char>;    //!< Type alias for string buffer using `char` type



            /**
             * \brief Basic file buffer interface
             */
            template<typename charT> class basic_file_buffer : public basic_buffer<charT>
            {
            public:
                using basic_buffer<charT>::end;

                /**
                 * \brief Open file
                 * \param name The name of the file to open
                 * \return Success/failure status
                 * \retval true If file opened successfully
                 * \retval false If file failed to open
                 */
                virtual bool open(std::string const& name) = 0;

                /**
                 * \brief Close the file
                 *
                 * After closing the buffer can't be used any more.
                 */
                virtual void close() = 0;

                /**
                 * \brief Check if the file is open or not
                 * \return The status of the file-buffer
                 * \retval true If the file-buffer is opened
                 * \retval false If the file-buffer is not opened
                 */
                virtual bool is_open() = 0;
            };

            using file_buffer = basic_file_buffer<char>;
        }
    }
}

#endif //RAVEN_LANG_BUFFER_H
