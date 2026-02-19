#include <gtest/gtest.h>
#include <iostream>
#include "move.hpp"

TEST(MoveTest, valueConstructorTest) {
    Move m(static_cast<uint16_t>(1ULL << 2));
    EXPECT_EQ(m.data(), static_cast<uint16_t>(0x4));
}
TEST(MoveTest, constructorTest) {
    Move m(Bitboard::Square(3 << 2), Bitboard::Square(3 << 2), promotion, queen );
    EXPECT_EQ(m.data(), 0xE30C);
}
TEST(MoveTest, objectSizeTest) {
    Move m(Bitboard::Square(3 << 2), Bitboard::Square(3 << 2), promotion, queen );
    EXPECT_EQ(sizeof(m), 2);
}
TEST(MoveTest, fromToEncodingTest) {
    for (int i{}; i < 64; i++) {
      for (int j{63}; j>=0; j-- ){
        Bitboard::Square from = static_cast<Bitboard::Square>(i);
        Bitboard::Square to = static_cast<Bitboard::Square>(j);
        Move m(from, to);
        EXPECT_EQ(m.from(), from);
        EXPECT_EQ(m.to(), to);
        EXPECT_EQ(m.type(), standard);
        EXPECT_EQ(m.promotionType(), knight);

      }
    }
} 