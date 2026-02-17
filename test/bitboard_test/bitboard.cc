#include <gtest/gtest.h>
#include <iostream>
#include "bitboard.hpp"




/// testing bitboards
TEST(BitboardTest, lsbReturnsCorrectIndexForRandomHex) {
	using namespace Bitboard;
	EXPECT_EQ(lsb(0x1ULL), 0);
    EXPECT_EQ(lsb(0x100ULL), 8);
    EXPECT_EQ(lsb(0x80000000ULL), 31);
    EXPECT_EQ(lsb(0xF000ULL), 12);
}
TEST(BitboardTest, lsbReturnsCorreectIndexForHex) {
   using namespace Bitboard;
   for (int i{}; i < 64; i++) {
     EXPECT_EQ((lsb(1ULL << i)), i);
   }
}
TEST(BitboardTest, lsbAssertsOnZeroValue) {
    using namespace Bitboard;
    #ifndef NDEBUG
        EXPECT_DEATH(lsb(0ULL), "");
    #else
        GTEST_SKIP() << "Skipping Death Test in Release mode (asserts disabled)";
    #endif
}
TEST(BitboardTest, msbReturnsCorrectIndexForRandomHex) {
	using namespace Bitboard;
	EXPECT_EQ(msb(0x1ULL), 0);
    EXPECT_EQ(msb(0x100ULL), 8);
    EXPECT_EQ(msb(0x80000000ULL), 31);
    EXPECT_EQ(msb(0xF000ULL), 15);
}
TEST(BitboardTest, msbReturnsCorreectIndexForHex) {
   using namespace Bitboard;
   for (int i{}; i < 64; i++) {
     EXPECT_EQ((msb(1ULL << i)), i);
   }
}
TEST(BitboardTest, msbAssertsOnZeroValue) {
    using namespace Bitboard;
    #ifndef NDEBUG
        EXPECT_DEATH(msb(0ULL), "");
    #else
        GTEST_SKIP() << "Skipping Death Test in Release mode (asserts disabled)";
    #endif
}
TEST(BitboardTest, msbAndLsbOnTheSameNumber) {
	using namespace Bitboard;
	bitboard b = (1ULL << 30) | (1ULL << 15) ;
	EXPECT_EQ(lsb(b), 15);
	EXPECT_EQ(msb(b), 30);
}
TEST(BitboardTest, setGetResetTest) {
	using namespace Bitboard;
	bitboard b = 0ULL;
	Square sq = e8 ;
    EXPECT_EQ((b, sq), 0);
	set_bit(b, sq);
	EXPECT_EQ((b, sq), 1);
	reset_bit(b, sq);
	EXPECT_EQ(get_bit(b, sq), 0);
}