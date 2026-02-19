#include <gtest/gtest.h>
#include <iostream>
#include "move.hpp"

TEST(MoveTest, constructorTest) {
    Move m(static_cast<uint16_t>(1ULL << 2));
    EXPECT_EQ(m.from(), 0x4);
}

TEST(MoveTest, fromToEncodingTest) {
    for (int i{}; i < 64; i++) {
      for (int j{63}; j>=0; j-- ){
        Bitboard::Square from = static_cast<Bitboard::Square>(i);
        Bitboard::Square to = static_cast<Bitboard::Square>(j);
        Move m(from, to);
        EXPECT_EQ(m.from(), from);
        EXPECT_EQ(m.to(), to);
      }
    }
}