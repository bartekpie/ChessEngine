#include <gtest/gtest.h>
#include "generator.hpp"
TEST(generatorTest, basicMoveListTest) {
    MoveList list;
    list += Move::makeMove(Bitboard::a1, Bitboard::a2);

};