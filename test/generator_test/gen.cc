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
          EXPECT_EQ(move.data(), m1.data());
        if (count == 1)
          EXPECT_EQ(move.data(), m2.data());
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
};
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
};
TEST(MoveListTest, AddingAfterReset) {
    MoveList list;
    Move m1 = Move::makeMove(Bitboard::a1, Bitboard::a2);
    Move m2 = Move::makeMove(Bitboard::h7, Bitboard::h8, MoveType::promotionBishop);
    
    list += m1;
    list += m2;
    list.reset();
    Move m3 = Move::makeMove(Bitboard::g2, Bitboard::g2);
    Move m4 = Move::makeMove(Bitboard::h2, Bitboard::g8, MoveType::promotionQueen);
    list += m3;
    list += m4;
    EXPECT_EQ(list.begin(), m3);
    EXPECT_EQ(list.begin() + 1, m4);
};
TEST(MoveListTest, AddingAfterMaxCapacity) {
    MoveList list;
    for (int i{}; i < 256; i++) 
      list += Move::makeMove(Bitboard::a1, Bitboard::a2);
   
    #ifndef NDEBUG
        EXPECT_DEATH(list += Move::makeMove(Bitboard::a1, Bitboard::a2);, "");
    #endif

};