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
TEST(Precompiled_directionsTest, checkingKnightMovesInTheLeftSideOfBoard) {
    auto test_case =precompiled_directions[16][knight];
    EXPECT_EQ(Bitboard::count_bits(test_case), 4);
    EXPECT_EQ(Bitboard::lsb(test_case), 1);
    Bitboard::reset_bit(test_case, Bitboard::Square(1));
    EXPECT_EQ(Bitboard::lsb(test_case), 10);
    Bitboard::reset_bit(test_case, Bitboard::Square(10));
    EXPECT_EQ(Bitboard::lsb(test_case), 26);
    Bitboard::reset_bit(test_case, Bitboard::Square(26));
    EXPECT_EQ(Bitboard::lsb(test_case), 33);
    Bitboard::reset_bit(test_case, Bitboard::Square(33));
    EXPECT_FALSE(test_case);
};
TEST(Precompiled_directionsTest, checkingKnightMovesInTheRigthSideOfBoard) {
    auto test_case =precompiled_directions[23][knight];
    EXPECT_EQ(Bitboard::count_bits(test_case), 4);
    EXPECT_EQ(Bitboard::lsb(test_case), 6);
    Bitboard::reset_bit(test_case, Bitboard::Square(6));
    EXPECT_EQ(Bitboard::lsb(test_case), 13);
    Bitboard::reset_bit(test_case, Bitboard::Square(13));
    EXPECT_EQ(Bitboard::lsb(test_case), 29);
    Bitboard::reset_bit(test_case, Bitboard::Square(29));
    EXPECT_EQ(Bitboard::lsb(test_case), 38);
    Bitboard::reset_bit(test_case, Bitboard::Square(38));
    EXPECT_FALSE(test_case);
};
TEST(Precompiled_directionsTest, checkingKnightMovesInTheEges) {
    // letfdown
    auto test_case =precompiled_directions[0][knight];
    EXPECT_EQ(Bitboard::count_bits(test_case), 2);
    EXPECT_EQ(Bitboard::lsb(test_case), 10);
    Bitboard::reset_bit(test_case, Bitboard::Square(10));
    EXPECT_EQ(Bitboard::lsb(test_case), 17);
    Bitboard::reset_bit(test_case, Bitboard::Square(17));
    EXPECT_FALSE(test_case);
    //right down
    test_case =precompiled_directions[7][knight];
    EXPECT_EQ(Bitboard::count_bits(test_case), 2);
    EXPECT_EQ(Bitboard::lsb(test_case), 13);
    Bitboard::reset_bit(test_case, Bitboard::Square(13));
    EXPECT_EQ(Bitboard::lsb(test_case), 22);
    Bitboard::reset_bit(test_case, Bitboard::Square(22));
    EXPECT_FALSE(test_case);
    //left up
    test_case =precompiled_directions[56][knight];
    EXPECT_EQ(Bitboard::count_bits(test_case), 2);
    EXPECT_EQ(Bitboard::lsb(test_case), 41);
    Bitboard::reset_bit(test_case, Bitboard::Square(41));
    EXPECT_EQ(Bitboard::lsb(test_case), 50);
    Bitboard::reset_bit(test_case, Bitboard::Square(50));
    EXPECT_FALSE(test_case);
    //right up
    test_case =precompiled_directions[63][knight];
    EXPECT_EQ(Bitboard::count_bits(test_case), 2);
    EXPECT_EQ(Bitboard::lsb(test_case), 46);
    Bitboard::reset_bit(test_case, Bitboard::Square(46));
    EXPECT_EQ(Bitboard::lsb(test_case), 53);
    Bitboard::reset_bit(test_case, Bitboard::Square(53));
    EXPECT_FALSE(test_case);
};
TEST(Precompiled_directionsTest, checkingKnightInMiddle) {
    Bitboard::bitboard test_case = precompiled_directions[Bitboard::d4][knight];
   
    EXPECT_EQ(Bitboard::count_bits(test_case), 8);

    EXPECT_EQ(Bitboard::lsb(test_case), Bitboard::c2);
    Bitboard::reset_bit(test_case, Bitboard::Square(Bitboard::c2));
    EXPECT_EQ(Bitboard::lsb(test_case), Bitboard::e2);
    Bitboard::reset_bit(test_case, Bitboard::Square(Bitboard::e2));
    EXPECT_EQ(Bitboard::lsb(test_case), Bitboard::b3);
    Bitboard::reset_bit(test_case, Bitboard::Square(Bitboard::b3));
    EXPECT_EQ(Bitboard::lsb(test_case), Bitboard::f3);
    Bitboard::reset_bit(test_case, Bitboard::Square(Bitboard::f3));
    EXPECT_EQ(Bitboard::lsb(test_case), Bitboard::b5);
    Bitboard::reset_bit(test_case, Bitboard::Square(Bitboard::b5));
    EXPECT_EQ(Bitboard::lsb(test_case), Bitboard::f5);
    Bitboard::reset_bit(test_case, Bitboard::Square(Bitboard::f5));
    EXPECT_EQ(Bitboard::lsb(test_case), Bitboard::c6);
    Bitboard::reset_bit(test_case, Bitboard::Square(Bitboard::c6));
    EXPECT_EQ(Bitboard::lsb(test_case), Bitboard::e6);
    Bitboard::reset_bit(test_case, Bitboard::Square(Bitboard::e6));
    EXPECT_EQ(test_case, 0);
};