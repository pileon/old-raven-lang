#include <gtest/gtest.h>
#include <lexer/buffer.h>

/**
 * \brief Test fixture for the string buffer tests
 */
class lexer_string_buffer_fixture : public ::testing::Test
{
protected:
    void SetUp() override final
    {
        buffer = new compiler::lexer::buffers::string_buffer(text, "foo");
    }

    void TearDown() override final
    {
        delete buffer;
    }

public:
    static const std::string text;

    compiler::lexer::buffers::string_buffer* buffer;
};
const std::string lexer_string_buffer_fixture::text = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";


TEST_F(lexer_string_buffer_fixture, get_first_char)
{
    EXPECT_EQ(buffer->get(), 'L');
}

TEST_F(lexer_string_buffer_fixture, get_five_chars)
{
    EXPECT_EQ(buffer->get(), 'L');
    EXPECT_EQ(buffer->get(), 'o');
    EXPECT_EQ(buffer->get(), 'r');
    EXPECT_EQ(buffer->get(), 'e');
    EXPECT_EQ(buffer->get(), 'm');
}

TEST_F(lexer_string_buffer_fixture, get_unget_get)
{
    EXPECT_EQ(buffer->get(), 'L');
    buffer->unget();
    EXPECT_EQ(buffer->get(), 'L');
}

TEST_F(lexer_string_buffer_fixture, get_putback_get)
{
    EXPECT_EQ(buffer->get(), 'L');
    buffer->putback('X');
    EXPECT_EQ(buffer->get(), 'X');
}
