#include <gtest/gtest.h>
#include <gray_code.h>

using namespace dl;

class GrayCodeFixture : public ::testing::Test
{
protected:
    using Encoder = GrayEncoder<uint32_t, uint32_t>;
    Encoder m_encoder;
};

TEST_F(GrayCodeFixture, BasicEncode) {
    EXPECT_EQ(m_encoder.encode(0u), 0);
    EXPECT_EQ(m_encoder.encode(1u), 1u);
    EXPECT_EQ(m_encoder.encode(2u), 3u);
    EXPECT_EQ(m_encoder.encode(3u), 2u);
    EXPECT_EQ(m_encoder.encode(4u), 6u);
}

TEST_F(GrayCodeFixture, BasicDecode) {
    EXPECT_EQ(m_encoder.decode(0u), 0);
    EXPECT_EQ(m_encoder.decode(1u), 1u);
    EXPECT_EQ(m_encoder.decode(3u), 2u);
    EXPECT_EQ(m_encoder.decode(2u), 3u);
    EXPECT_EQ(m_encoder.decode(6u), 4u);
}
