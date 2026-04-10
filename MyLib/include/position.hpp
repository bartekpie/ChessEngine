#pragma once
#include "bitboard.hpp"
#include "move.hpp"
#include <array>
#include <string>
#include <cassert>
#include <unordered_map>
#include <vector>
enum class PiecesType {
    pawn= 0 , knight, bishop, rook, queen, king
};
enum class Pieces {
    white_pawn = 0, white_knight, white_bishop, white_rook, white_queen, white_king,
    black_pawn    , black_knight, black_bishop, black_rook, black_queen, black_king, size_of_pieces
};
enum class CastilingRights {
    white_king_side, white_queen_side, black_king_side, black_queen_side
};
enum class Color {
    black = 0, white
};
struct MoreInfo {
    Bitboard::Square doublePushedMove_;
    Pieces capturedPiece_;
    Bitboard::Square capturedSquare_;
    bool castlingRights[4];  
    MoreInfo() : 
      doublePushedMove_(Bitboard::a1),
      capturedPiece_(Pieces::size_of_pieces),
      capturedSquare_(Bitboard::a1) {}
};
// for heap alocated information
class MoreInfoManager {
    private:
      std::vector<MoreInfo> data_;
      size_t current {0};
    public:
      explicit MoreInfoManager(size_t max_depth) {
        data_.reserve(max_depth);
      }
      void add(MoreInfo moreinfo) {
        data_[current] = moreinfo;
        current++;
      }
      MoreInfo pop() {
        assert(data_.size() > 0);
        return data_[current--];
      }
      MoreInfo last() const {
        //assert(data_.size() > 0);
        return data_[current];
      }
      int size() {
        return data_.size();
      }
};
class Position { 
    private:
      std::array<Bitboard::bitboard, int(Pieces::size_of_pieces)> board_;
      std::array<Bitboard::bitboard, 2> colorBoard_;
      Bitboard::bitboard emptySpaces_;
      Color sideToMove ;
      MoreInfoManager moreInfoManager_;
    public :
      Position(): board_{}, colorBoard_{}, emptySpaces_{~0ULL}, moreInfoManager_{100} {}
      Position(const std::string& fen_position);
      void loadFromFEN(const std::string& fen_position);
      void clear();
      Color getSideToMove() const {return sideToMove;}
      Bitboard::bitboard getPieces(Pieces piece) const {return board_[int(piece)];}
      template <Color color> Bitboard::bitboard getPiecesByColor(PiecesType piece) const;
      template <PiecesType piece> Bitboard::bitboard getOurs() const;
      inline Bitboard::bitboard getOurs() const;
      template <PiecesType piece> Bitboard::bitboard getOpponents() const;
      inline Bitboard::bitboard getOpponents() const;
      inline Bitboard::bitboard getEmptySpaces() const;
      Pieces getCurrPiece(Bitboard::Square from);
      MoreInfo getMoreInfo() const;
      void addMoreInfo(MoreInfo info);
      void simulate_move(Move move);
      void undo_move();

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
    int offset = sideToMove == Color::black ? 6 : 0;
    return board_[int(piece) + offset];
}
inline void Position::clear()
{
    board_.fill(0ULL);
    colorBoard_.fill(0ULL);
    emptySpaces_ = ~0ULL;
    
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
            colorBoard_[int(Color::white)] |= color == Color::white ? (1ULL << position) : 0ULL;
            colorBoard_[int(Color::black)] |= color == Color::black ? (1ULL << position) : 0ULL;
            emptySpaces_ &= ~(1ULL << position);
            file++;
        }
        index++;
        
    }
    assert(++index < fen_position.length());
    sideToMove = fen_position[index] == 'w' ? Color::white : Color::black;

}
inline Position::Position(const std::string& fen_position) : moreInfoManager_(100)
{
    loadFromFEN(fen_position);

}
inline Bitboard::bitboard Position::getOurs() const
{
    auto r = sideToMove == Color::white ? colorBoard_[int(Color::white)] : colorBoard_[int(Color::black)];
    return r;
}
template <PiecesType piece>
Bitboard::bitboard Position::getOpponents() const {
    int offset = sideToMove == Color::white ? 6 : 0;
    return board_[int(piece) + offset];
}
inline Bitboard::bitboard Position::getOpponents() const {
  auto r = sideToMove == Color::white ? colorBoard_[int(Color::black)] : colorBoard_[int(Color::white)];
  return r;
}
inline Bitboard::bitboard Position::getEmptySpaces() const {
  return emptySpaces_;
}
inline MoreInfo Position::getMoreInfo() const {
    return moreInfoManager_.last();
}
inline void Position::addMoreInfo(MoreInfo info) {
    moreInfoManager_.add(info);

}