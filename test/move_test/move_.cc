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
TEST(MoveTest, promotionTypeMetohodTest) {
    Move m(Bitboard::a1, Bitboard::a1, promotion, rook);
    EXPECT_EQ(m.promotionType(), rook);

}
TEST(MoveTest, typeMetohodTest) {
    Move m(Bitboard::a1, Bitboard::a1, castle);
    EXPECT_EQ(m.type(), castle);
}
TEST(MoveTest, objectSizeTest) {
    Move m(Bitboard::Square(3 << 2), Bitboard::Square(3 << 2), promotion, queen );
    EXPECT_EQ(sizeof(m), 2);
}
TEST(MoveTest, enumOutOfBounds) {
  #ifndef NDebug
    EXPECT_DEATH(Move m(static_cast<Bitboard::Square>(70), Bitboard::a1), "");
    EXPECT_DEATH(Move m(Bitboard::a1,static_cast<Bitboard::Square>(70)), "");
    EXPECT_DEATH(Move m(Bitboard::a1,Bitboard::a1, static_cast<MoveType>(10)), "");
    EXPECT_DEATH(Move m(Bitboard::a1,Bitboard::a1, MoveType::promotion, static_cast<PromotionPiece>(10)), "");

  #endif
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
TEST(MoveTest, staticMakeMoveTest) {
  Move m = Move::makeMove(Bitboard::b1, Bitboard::b1, standard);
  Move r(Bitboard::b1, Bitboard::b1, standard);
  EXPECT_EQ(m.data(),r.data());
}