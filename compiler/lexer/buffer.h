#ifndef RAVEN_LANG_BUFFER_H
#define RAVEN_LANG_BUFFER_H

#include <string>

namespace compiler
{
    namespace lexer
    {
        /**
         * \brief Base class to handle buffering of input for the lexer
         * \param charT The character type for the buffering
         *
         * The buffer scheme uses two buffers, one "current" buffer and one that was
         * the previous buffer. When the end of the current buffer is reached the
         * previous buffer is filled with data, and sthen et to the current buffer.
         *
         * Using double-buffering like this gives us large headroom for unget/putback,
         * which helps the lexer handle advanced multi-character tokens.
         */
        template<typename charT>
        class basic_buffer
        {
        public:
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
             * \note Made virtual because it's overridden by string_buffer
             */
            virtual charT get();

            /**
             * \brief "Unget" the last character returned by get()
             * \note Made virtual because it's overridden by string_buffer
             */
            virtual void unget();

            /**
             * \brief Put back a character into the buffer
             * \param ch The character to put back
             * \note Made virtual because it's overridden by string_buffer
             *
             * Put back a character into the buffer. This will be the next character
             * returned by get().
             */
            virtual void putback(charT const& ch);

            /**
             * \brief Returned by get() to mark the end of the input
             */
            static constexpr charT end = reinterpret_cast<charT>(-1);

        protected:
            basic_buffer()
                : name_{}
            {}

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
            virtual size_t fill_buffer(charT* buffer, size_t const length) = 0;

        private:
            std::string name_;  //!< The name of the buffer
            size_t size_;       //!< The size of a single buffer
            charT* buffer_;     //!< The double buffer
            charT* endfirst_;   //!< The end of the first buffer
            charT* endsecond;   //!< The end of the second buffer
            charT* end_;        //!< If not nullptr, marks the final end of the input
            charT* current_;    //!< Current position in the buffer(s)
        };

        template<typename charT>
        class string_buffer : public basic_buffer<charT>
        {
        public:
            string_buffer()
                : basic_buffer<charT>()
            {}
        };
    }
}

#endif //RAVEN_LANG_BUFFER_H
