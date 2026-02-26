#pragma once
#include "bitboard.hpp"
#include <array>
#include <string>
#include <cassert>
#include <unordered_map>
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
      Position(): board_{}, whitePieces_{}, blackPieces_{}, emptySpaces_{~0ULL} {}
      Position(const std::string& fen_position);
      void loadFromFEN(const std::string& fen_position);
      void clear();
      Bitboard::bitboard getPieces(Pieces piece) const {return board_[int(piece)];}
      template <Color color> Bitboard::bitboard getPiecesByColor(PiecesType piece) const;
      template <PiecesType piece> Bitboard::bitboard getOurs() const;
      inline Bitboard::bitboard getOurs() const;
      template <PiecesType piece> Bitboard::bitboard getOpponents() const;
      inline Bitboard::bitboard getOpponents() const;
      inline Bitboard::bitboard getEmptySpaces() const;

};
template <Color color>
Bitboard::bitboard Position::getPiecesByColor(PiecesType piece) const {
    if constexpr (color == Color::white)
      return board_[int(piece)];
    else {
        return board_[int(piece) + 6];
    }
}
template <PiecesType piece>
Bitboard::bitboard Position::getOurs() const {
    int offset = sideToMove == Color::black ? 0 : 6;
    return board_[int(piece) + offset];
}
inline void Position::clear()
{
    whitePieces_ = 0ULL;
    blackPieces_ = 0ULL;
    emptySpaces_ = ~0ULL;
    board_.fill(0ULL);
}
inline void Position::loadFromFEN(const std::string& fen_position)
{
    clear();
    auto rank{7};
    auto file{0};
    auto index{0};
    std::unordered_map<char, std::pair<Pieces, Color>> charToPiece {
        {'P', {Pieces::white_pawn,   Color::white}},
        {'N', {Pieces::white_knight, Color::white}},
        {'B', {Pieces::white_bishop, Color::white}},
        {'R', {Pieces::white_rook,   Color::white}},
        {'Q', {Pieces::white_queen,  Color::white}},
        {'K', {Pieces::white_king,   Color::white}},
        {'p', {Pieces::black_pawn,   Color::black}},
        {'n', {Pieces::black_knight, Color::black}},
        {'b', {Pieces::black_bishop, Color::black}},
        {'r', {Pieces::black_rook,   Color::black}},
        {'q', {Pieces::black_queen,  Color::black}},
        {'k', {Pieces::black_king,   Color::black}}
    };
    for (char piece: fen_position) {
        if (piece == ' '){
            break;
        }
        else if (piece == '/'){
            rank--;
            file = 0;
        } 
        else if(isdigit(piece)){
            file += piece - '0';
        }
        else {
            assert(charToPiece.find(piece) != charToPiece.end());
            auto [piecetype, color] = charToPiece[piece];
            auto position = Bitboard::Square(rank*8 + file);
            Bitboard::set_bit(board_[int(piecetype)], position);
            whitePieces_ |= color == Color::white ? 1ULL << position : 0ULL;
            blackPieces_ |= color == Color::black ? 1ULL << position : 0ULL;
            emptySpaces_ &= ~(1ULL << position);
            file++;
        }
        index++;
        
    }
    assert(++index < fen_position.length());
    sideToMove = fen_position[index] == 'w' ? Color::white : Color::black;

}
inline Position::Position(const std::string& fen_position)
{
    loadFromFEN(fen_position);

}
inline Bitboard::bitboard Position::getOurs() const
{
    auto r = sideToMove == Color::white ? whitePieces_ : blackPieces_;
    return r;
}
template <PiecesType piece>
Bitboard::bitboard Position::getOpponents() const {
    int offset = sideToMove == Color::white ? 6 : 0;
    return board_[int(piece) + offset];
}
inline Bitboard::bitboard Position::getOpponents() const {
  auto r = sideToMove == Color::white ? blackPieces_ : whitePieces_;
  return r;
}
inline Bitboard::bitboard Position::getEmptySpaces() const {
  return emptySpaces_;
}