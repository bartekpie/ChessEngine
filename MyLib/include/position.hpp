#pragma once
#include "bitboard.hpp"
#include "move.hpp"
#include <array>
#include <string>
#include <cassert>
#include <unordered_map>
#include <vector>
#include <sstream>
#include <algorithm>
enum class PiecesType {
    pawn= 0 , knight, bishop, rook, queen, king
};
enum class Pieces {
    white_pawn = 0, white_knight, white_bishop, white_rook, white_queen, white_king,
    black_pawn    , black_knight, black_bishop, black_rook, black_queen, black_king, size_of_pieces
};
enum CastilingRights : uint8_t {
    white_king_side = 1,
    white_queen_side = 1 << 1,
    black_king_side = 1 << 2,
    black_queen_side = 1 << 3
};
enum class Color {
    black = 0, white
};
struct MoreInfo {
    Bitboard::Square doublePushedMove_;
    Bitboard::Square afterPassantSquare_;
    Pieces capturedPiece_;
    Pieces movedPiece_;
    Bitboard::Square capturedSquare_;
    uint8_t castlingRights_;  
    uint8_t nonCaptureMoveCount_;
    uint8_t moveCount_;
    Move lastMove_;
    MoreInfo() : 
      doublePushedMove_(Bitboard::a1),
      capturedPiece_(Pieces::size_of_pieces),
      afterPassantSquare_(Bitboard::a1),
      capturedSquare_(Bitboard::a1),
      castlingRights_(0),
      nonCaptureMoveCount_(0),
      moveCount_(0),
      lastMove_(0) {}
    bool operator== (const MoreInfo& m) const {
       if (doublePushedMove_    == m.doublePushedMove_ &&
           afterPassantSquare_  == m.afterPassantSquare_ &&
           capturedPiece_       == m.capturedPiece_ &&
           movedPiece_          == m.movedPiece_ &&
           capturedSquare_      == m.capturedSquare_ &&
           castlingRights_      == m.castlingRights_ &&
           nonCaptureMoveCount_ == m.nonCaptureMoveCount_ &&
           moveCount_           == m.moveCount_ &&
           lastMove_            == m.lastMove_) return true;
           return false;
    }

};
// for heap alocated information
class MoreInfoManager {
    private:
      std::vector<MoreInfo> data_;
      size_t current {0};
    public:
      explicit MoreInfoManager(size_t max_depth) : data_(max_depth) {}
      void add(MoreInfo moreinfo) {
        current++;
        data_[current] = moreinfo;
      }
      bool operator==(const MoreInfoManager& m) const {
        if (data_[current] == m.data_[m.current]) return true;
        return false;
      }
      MoreInfo pop() {
        assert(current > 0);
        return data_[current--];
      }
      MoreInfo last() const {
        //assert(data_.size() > 0);
        return data_[current];
      }
      size_t size() {
        return current;
      }
      void clear() {
         current = 0;
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
      Position();
      Position(const std::string& fen_position);
      bool operator==(const Position& p) const;
      int loadFromFEN(const std::string& fen_position);
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
    moreInfoManager_.clear();
    
}
inline int Position::loadFromFEN(const std::string& fen_position)
{
    clear();

    auto failed = [&](){clear(); return 0;};

    std::istringstream i(fen_position);
    std::vector<std::string> seperated;
    std::string current;
    while (i >> current)
      seperated.push_back(current);
    if (seperated.size() != 6) {
        return failed();
    }

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
    auto rank{7};
    auto file{0};
    auto index{0};
    for (char piece: seperated[0]) {
        
        if (piece == '/'){
            if (file != 8) return failed();
            rank--;
            file = 0;
        } 
        else if(std::isdigit(static_cast<unsigned char>(piece))){
            file += piece - '0';
            if (file > 8) failed();
        }
        else {
            if (charToPiece.find(piece) == charToPiece.end()) return failed();
            auto [piecetype, color] = charToPiece[piece];
            auto position = Bitboard::Square(rank*8 + file);
            Bitboard::set_bit(board_[int(piecetype)], position);
            colorBoard_[int(Color::white)] |= color == Color::white ? (1ULL << position) : 0ULL;
            colorBoard_[int(Color::black)] |= color == Color::black ? (1ULL << position) : 0ULL;
            emptySpaces_ &= ~(1ULL << position);
            file++;
        }
    }

    sideToMove = seperated[1] == "w" ? Color::white : Color::black;

    MoreInfo moreinfo;

    if (std::find(seperated[2].begin(), seperated[2].end(), 'Q') != seperated[2].end()) 
       moreinfo.castlingRights_ |= CastilingRights::white_queen_side;
    if (std::find(seperated[2].begin(), seperated[2].end(), 'q') != seperated[2].end()) 
       moreinfo.castlingRights_ |= CastilingRights::black_queen_side;
    if (std::find(seperated[2].begin(), seperated[2].end(), 'K') != seperated[2].end()) 
       moreinfo.castlingRights_ |= CastilingRights::white_king_side;
    if (std::find(seperated[2].begin(), seperated[2].end(), 'k') != seperated[2].end()) 
       moreinfo.castlingRights_ |= CastilingRights::black_king_side;
    
    if (seperated[3].size() == 2) {
       if (!isdigit(seperated[3][1]))
          return failed();
       seperated[3][0] = std::tolower(seperated[3][0]);
       if (!(seperated[3][0] >= 'a' && seperated[3][0] <='h'))
          return failed();
       int file = seperated[3][0] - 'a';
       int rank = seperated[3][1] - '1';
       if (rank > 7) return failed();
       moreinfo.doublePushedMove_ = Bitboard::Square(rank * 8 + file);
    }
    else if (seperated[3] == "-")
    {}
    else {
       return failed();
    }
     
    if (std::all_of(seperated[4].begin(), seperated[4].end(), [](unsigned char c){ return std::isdigit(c); })) {
        moreinfo.nonCaptureMoveCount_ = std::stoi(seperated[4]);
    }

    if (std::all_of(seperated[5].begin(), seperated[5].end(), [](unsigned char c){ return std::isdigit(c); })) {
        moreinfo.moveCount_ = std::stoi(seperated[5]);
    }

    this->moreInfoManager_.add(moreinfo);

    return 1;
}
inline Position::Position() : moreInfoManager_(100)
{
    loadFromFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w  KkQq - 0 1");
}
inline Position::Position(const std::string &fen_position) : moreInfoManager_(100)
{
    loadFromFEN(fen_position);

}
inline bool Position::operator==(const Position &p) const
{
    if (board_           == p.board_ &&
        colorBoard_      == p.colorBoard_ &&
        emptySpaces_     == p.emptySpaces_ &&
        sideToMove       == p.sideToMove &&
        moreInfoManager_ == p.moreInfoManager_) return true;
        return false;
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