#include <gtest/gtest.h>
#include <algorithm>
#include "position.hpp"
#include "move.hpp"
#include "bitboard.hpp"
#include "generator.hpp"
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
TEST(simulate_move, simulate_castling_move) {
  std::string fen_position = "8/8/8/8/8/8/8/4K2R w KQkq - 0 1";
  Position pos(fen_position);

  pos.simulate_move(Move::makeMove(Bitboard::e1, Bitboard::h1, MoveType::castle));
 
  EXPECT_TRUE(Bitboard::get_bit(pos.getPieces(Pieces::white_rook), Bitboard::f1));
  EXPECT_EQ(Bitboard::count_bits(pos.getPieces(Pieces::white_rook)), 1);

  EXPECT_TRUE(Bitboard::get_bit(pos.getPieces(Pieces::white_king), Bitboard::g1));
  EXPECT_EQ(Bitboard::count_bits(pos.getPieces(Pieces::white_king)), 1);

  EXPECT_TRUE(Bitboard::get_bit(pos.getOpponents(), Bitboard::f1));
  EXPECT_EQ(Bitboard::count_bits(pos.getOpponents()), 2);

  EXPECT_TRUE(Bitboard::get_bit(pos.getOpponents(), Bitboard::g1));

  Bitboard::bitboard b = 1ULL << Bitboard::f1 | 1ULL << Bitboard::g1;

  EXPECT_EQ(pos.getEmptySpaces(), ~b);

  for (int i = 0; i <= static_cast<int>(PiecesType::king); i++) 
        EXPECT_EQ(pos.getPiecesByColor<Color::black>(static_cast<PiecesType>(i)), 0ULL);

  std::vector<int> pom {0,1,2,4};
  for (auto i : pom)
    EXPECT_EQ(pos.getPiecesByColor<Color::white>(static_cast<PiecesType>(i)), 0ULL);

};
TEST (simulate_move, simulate_and_undo_standard_move) {
  Bitboard::print_bitboard(12);
  std::string fen_position = "8/8/8/8/8/8/P7/8 w - - 0 1";
  Position pos(fen_position);
  Position f(fen_position);
  pos.simulate_move(Move::makeMove(Bitboard::a2, Bitboard::a3));
  pos.undo_move();
  EXPECT_TRUE(f == pos);

};
TEST (simulate_move, simulate_and_undo_capture_move) {
  Bitboard::print_bitboard(12);
  std::string fen_position = "8/8/8/8/8/1p6/P7/8 w - - 0 1";
  Position pos(fen_position);
  Position f(fen_position);
  pos.simulate_move(Move::makeMove(Bitboard::a2, Bitboard::b3,MoveType::capture));
  pos.undo_move();
  EXPECT_TRUE(f == pos);

};
TEST(simulate_move, simulate_and_undo_promotion_move) {
  std::string fen_position = "8/P7/8/8/8/8/8/8 w - - 0 1";
  Position pos(fen_position);
  Position original(fen_position);

  pos.simulate_move(Move::makeMove(Bitboard::a7, Bitboard::a8, MoveType::promotionQueen));
  pos.undo_move();

  EXPECT_TRUE(original == pos);
};
TEST(simulate_move, simulate_and_undo_en_passant_move) {
  std::string fen_position = "8/8/8/3pP3/8/8/8/8 w - d5 0 1";
  Position pos(fen_position);
  Position original(fen_position);

  pos.simulate_move(Move::makeMove(Bitboard::e5, Bitboard::d6, MoveType::passant));
  pos.undo_move();

  EXPECT_TRUE(original == pos);
}
TEST(simulate_move, simulate_and_undo_castling_move) {
  std::string fen_position = "r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1";
  Position pos(fen_position);
  Position original(fen_position);

  pos.simulate_move(Move::makeMove(Bitboard::e1, Bitboard::h1, MoveType::castle));
  pos.undo_move();

  EXPECT_TRUE(original == pos);
};
TEST(LegalMoves, myTest) {
    const std::string pos = "rnb1kbnr/1p1ppppp/p1p5/q7/8/3P4/PPP1PPPP/RNBQKBNR w KQkq - 0 4";
    
    MoveList list;
    Position position(pos);
    position.simulate_move(2608);
    position.simulate_move(1227);
    position.simulate_move(2738);
    position.simulate_move(708);
    generate_all_moves(position, list);
    EXPECT_EQ(list.size(), 0);
    
};