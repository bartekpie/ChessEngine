#include <gtest/gtest.h>
#include <algorithm>
#include "position.hpp"
#include "move.hpp"
#include "bitboard.hpp"
#include <string>

TEST (simulate_move, simulate_standard_move) {
  Position basics;
  Position pos;
  pos.simulate_move(Move::makeMove(Bitboard::d2, Bitboard::d4));

  EXPECT_EQ(pos.getSideToMove(), Color::black);

  EXPECT_TRUE(Bitboard::get_bit(pos.getEmptySpaces(), Bitboard::d2));
  EXPECT_FALSE(Bitboard::get_bit(pos.getEmptySpaces(), Bitboard::d4));

  Bitboard::bitboard white_pawns = 0xFF00 | 1ULL << int(Bitboard::d4);
  Bitboard::reset_bit(white_pawns, Bitboard::d2);
  EXPECT_EQ(pos.getPiecesByColor<Color::white>(PiecesType::pawn), white_pawns);
  Bitboard::bitboard white_pieces = white_pawns | 0xFF;
  EXPECT_EQ(pos.getOpponents(), white_pieces);
  for (int i = int(PiecesType::knight); i <= static_cast<int>(PiecesType::king); i++) 
    EXPECT_EQ(pos.getPiecesByColor<Color::white>(static_cast<PiecesType>(i)), basics.getPiecesByColor<Color::white>(static_cast<PiecesType>(i)));
  
  EXPECT_EQ(pos.getOurs(), basics.getOpponents());
  for (int i = 0; i <= static_cast<int>(PiecesType::king); i++) 
    EXPECT_EQ(pos.getPiecesByColor<Color::black>(static_cast<PiecesType>(i)), basics.getPiecesByColor<Color::black>(static_cast<PiecesType>(i)));
  
};
TEST (simulate_move, simulate_capture_move) {
  std::string fen_position = "8/p7/8/8/8/8/8/R7 w - - 0 1";
  Position basics;
  Position pos(fen_position);
  pos.simulate_move(Move::makeMove(Bitboard::a1, Bitboard::a7, MoveType::capture));

  EXPECT_EQ(pos.getSideToMove(), Color::black);

  EXPECT_EQ(pos.getOurs(), 0ULL);

  EXPECT_EQ(pos.getOpponents(), 1ULL << int(Bitboard::a7));
  EXPECT_EQ(pos.getOpponents<PiecesType::rook>(), 1ULL << int(Bitboard::a7));
  
  EXPECT_EQ(pos.getEmptySpaces(), ~(1ULL << int(Bitboard::a7)));

  for (int i = 0; i <= static_cast<int>(PiecesType::king); i++) 
    EXPECT_EQ(pos.getPiecesByColor<Color::black>(static_cast<PiecesType>(i)), 0ULL);  
  
};
TEST (simulate_move, simulate_passant_move) {

    std::string fen_position = "8/8/8/8/3pP3/8/8/8 b - e4 0 1";
    Position basics;
    Position pos(fen_position);

    pos.simulate_move(Move::makeMove(Bitboard::d4, Bitboard::e3,MoveType::passant));

    EXPECT_EQ(pos.getEmptySpaces(), ~(1ULL << int(Bitboard::e3)));

    EXPECT_EQ(pos.getOpponents<PiecesType::pawn>(), 1ULL << int(Bitboard::e3));
    EXPECT_EQ(pos.getOurs(), 0ULL);
    
    for (int i = 0; i <= static_cast<int>(PiecesType::king); i++) 
        EXPECT_EQ(pos.getPiecesByColor<Color::white>(static_cast<PiecesType>(i)), 0ULL);
    for (int i = 1; i <= static_cast<int>(PiecesType::king); i++) 
        EXPECT_EQ(pos.getPiecesByColor<Color::black>(static_cast<PiecesType>(i)), 0ULL);
    
};