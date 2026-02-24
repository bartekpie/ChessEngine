#include "bitboard.hpp"
#include <array>
enum class PiecesType {
    pawn= 0 , knight, bishop, rook, queen, king
};
enum class Pieces {
    white_pawn = 0, white_knight, white_bishop,white_rook, white_queen, white_king,
    black_pawn, black_knight, black_bishop,black_rook, black_queen, black_king, size_of_pieces
};
enum class Color {
    black = 0, white
};
class Position { 
    private:
      std::array<Bitboard::bitboard, int(Pieces::size_of_pieces)> board_;
      Bitboard::bitboard whitePieces_;
      Bitboard::bitboard blackPieces_;
      Bitboard::bitboard emptySpaces_;
      Color sideToMove ;
    public :
      Bitboard::bitboard getPieces(Pieces piece) const {return board_[int(piece)];}
      template <Color color> Bitboard::bitboard getPiecesByColor(PiecesType piece) const;
      template <PiecesType piece> Bitboard::bitboard getOurs() const;
      Bitboard::bitboard getOurs() const;
      template <PiecesType piece> Bitboard::bitboard getOpponents() const;
      Bitboard::bitboard getOpponents() const;

};
template <Color color>
Bitboard::bitboard Position::getPiecesByColor(PiecesType piece) const {
    constexpr int offset = color == Color::white ? 0 : 6;
    return board_[int(piece) + offset];
}
template <PiecesType piece>
Bitboard::bitboard Position::getOurs() const {
    constexpr int offset = sideToMove == Color::black ? 0 : 6;
    return board_[int(piece) + offset];
}
Bitboard::bitboard Position::getOurs() const {
  auto r = sideToMove == Color::white ? whitePieces_ : blackPieces_;
  return r;
}
template <PiecesType piece>
Bitboard::bitboard Position::getOpponents() const {
    constexpr int offset = sideToMove == Color::white ? 0 : 6;
    return board_[int(piece) + offset];
}
Bitboard::bitboard Position::getOpponents() const {
  auto r = sideToMove == Color::white ? blackPieces_ : whitePieces_;
  return r;
}