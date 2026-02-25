#include <gtest/gtest.h>
#include "generator.hpp"
TEST(MoveListTest, AddsMovesCorrectly) {
    MoveList list;

    Move m1 = Move::makeMove(Bitboard::a1, Bitboard::a2);
    Move m2 = Move::makeMove(Bitboard::h7, Bitboard::h8, MoveType::promotionBishop);

    list += m1;
    list += m2;

    int count = 0;
    for (const auto& move : list) {
        if (count == 0)
          EXPECT_EQ(move, m1);
        if (count == 1)
          EXPECT_EQ(move, m2);
        count++;
        
    }
    EXPECT_EQ(count, 2);
};
TEST(MoveListTest, EmptyListIteration) {
    MoveList list;
    
    int count = 0;
    for (auto move : list) 
        count++;
    
    EXPECT_EQ(count, 0);
    EXPECT_EQ(list.begin(), list.end());
}
TEST(MoveListTest, IterationAfterReset) {
    MoveList list;
    Move m1 = Move::makeMove(Bitboard::a1, Bitboard::a2);
    Move m2 = Move::makeMove(Bitboard::h7, Bitboard::h8, MoveType::promotionBishop);
    
    list += m1;
    list += m2;
    list.reset();
    int count = 0;
    for (auto move : list) {
        count++;
    }
    EXPECT_EQ(count, 0);
    EXPECT_EQ(list.begin(), list.end());
}