#include <gtest/gtest.h>
#include "bitboard.hpp"




/// testing bitboards
TEST(BitboardTest, LsbReturnsCorrectIndexForRandomHex) {
	using namespace Bitboard;
	EXPECT_EQ(lsb(0x1ULL), 0);
    EXPECT_EQ(lsb(0x100ULL), 8);
    EXPECT_EQ(lsb(0x80000000ULL), 31);
    EXPECT_EQ(lsb(0xF000ULL), 12);
	EXPECT_EQ(lsb(1ULL << 63), 63);
}


TEST(BitboardTest, LsbAssertsOnZeroValue) {
    using namespace Bitboard;
    #ifndef NDEBUG
        EXPECT_DEATH(lsb(0ULL), "Bitboard cannot be zero");
    #else
        GTEST_SKIP() << "Skipping Death Test in Release mode (asserts disabled)";
    #endif
}
