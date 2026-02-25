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
    EXPECT_EQ(list.begin()->data(), m3.data());
    EXPECT_EQ((list.begin() + 1)->data(), m4.data());
};
TEST(MoveListTest, AddingAfterMaxCapacity) {
    MoveList list;
    for (int i{}; i < 256; i++) 
      list += Move::makeMove(Bitboard::a1, Bitboard::a2);
   
    #ifndef NDEBUG
        EXPECT_DEATH(list += Move::makeMove(Bitboard::a1, Bitboard::a2);, "");
    #endif

};
TEST(MoveListTest, BitboardToMovesWithEmptyBitboard) {
    MoveList list;
    list.bitboardToMoves(Bitboard::a1, 0ULL);
    int count{};
    for (const auto& move : list) 
        count++;
    EXPECT_EQ(count, 0);

};
TEST(MoveListTest, BitboardToMovesTest) {
    MoveList list;
    Bitboard::Square from = Bitboard::e2;
    Bitboard::bitboard moves{};
    Bitboard::set_bit(moves, Bitboard::e4);
    Bitboard::set_bit(moves, Bitboard::d3);
    Bitboard::set_bit(moves, Bitboard::f3);
    
    list.bitboardToMoves(from, moves, MoveType::standard);

    int count = std::distance(list.begin(), list.end());
    EXPECT_EQ(count, 3);

    
    std::vector<Bitboard::Square> targetSquares;
    for (const auto& move : list) {
        targetSquares.push_back(move.to());
        EXPECT_EQ(move.from(), from);
    }
    
    EXPECT_TRUE(std::find(targetSquares.begin(), targetSquares.end(), Bitboard::e4) != targetSquares.end());
    EXPECT_TRUE(std::find(targetSquares.begin(), targetSquares.end(), Bitboard::d3) != targetSquares.end());
    EXPECT_TRUE(std::find(targetSquares.begin(), targetSquares.end(), Bitboard::f3) != targetSquares.end());

};