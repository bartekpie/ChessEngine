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

//Test sprawdzający assert dla wartości 0 (Death Test)
/*
TEST(BitboardTest, LsbAssertsOnZeroValue) {
    
    #ifndef NDEBUG
        EXPECT_DEATH(get_lsb(0ULL), "Bitboard cannot be zero");
    #else
        GTEST_SKIP() << "Skipping Death Test in Release mode (asserts disabled)";
    #endif
}
*/