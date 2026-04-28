#pragma once
#include <iostream>
#include <cassert>
#include "bitboard.hpp"
// enums only used strictly with Move class
// cannot be used as table index only inforamtion holders.
// cannot iterate thru them big diffrence beetwen these numbers
// first is 0 next is bigger than million
// undefined behaviour when used inccorectly nessesary for efficiency though
enum MoveType : uint16_t{
    standard, 
    capture = 1 << 12,
    passant = 2 << 12,
    castle = 3 << 12,
    promotionKnight = 4 << 12,  
    promotionBishop = 5 << 12,  
    promotionRook = 6 << 12,  
    promotionQueen = 7 << 12,  
};

// Move class responsibilty is packing, and holding as much information about move as possible
// and extracting that information efficiently. 
class Move {
    private :
      uint16_t data_{};
    public :
      Move(): data_(0){};
      constexpr Move(uint16_t move);
      constexpr Move(Bitboard::Square from, Bitboard::Square to, MoveType type = standard); 
      bool operator==(const Move& move) const;
      constexpr MoveType type() const;
      constexpr Bitboard::Square from() const;
      constexpr Bitboard::Square to() const;
      constexpr uint16_t data() const;
      std::string toString() const;
      static constexpr Move makeMove(Bitboard::Square from, Bitboard::Square to, MoveType type = standard);
      
};
constexpr Move::Move(uint16_t move) : 
  data_(move){}

constexpr Move::Move(Bitboard::Square from, Bitboard::Square to, MoveType type) : 
  data_(from | to << 6 | type ) {
  assert(from >= Bitboard::a1 && from <= Bitboard::h8);
  assert(to >= Bitboard::a1 && to <= Bitboard::h8);
  assert(type == MoveType::standard||   // values not one by one diifrent aproach than above nessesary
    (type >= MoveType::capture && type <= MoveType::promotionQueen)); 
}
inline bool Move::operator==(const Move &move) const
{
    if (data_ == move.data_) return true;
    return false;
}
constexpr MoveType Move::type() const
{
    return( MoveType(data_  & 0x7000) );
}
constexpr uint16_t Move::data() const {
    return data_;
}
inline std::string Move::toString() const
{
    std::string move_str;
    auto from_square = this->from();
    auto to_square = this->to();
    move_str += char('a' + from_square % 8);
    move_str += char('1' + from_square / 8);
    move_str += char('a' + to_square % 8);  
    move_str += char('1' + to_square / 8);
    switch (this->type()) {
        case MoveType::standard:
        case MoveType::capture:
        case MoveType::passant:
        case MoveType::castle:
            break;
        case MoveType::promotionKnight:
            move_str += 'n';
            break;
        case MoveType::promotionBishop:
            move_str += 'b';
            break;
        case MoveType::promotionRook:
            move_str += 'r';
            break;
        case MoveType::promotionQueen:
            move_str += 'q';
            break;
    }
    return move_str;
}
constexpr Bitboard::Square Move::from() const
{
    return Bitboard::Square(data_ & 0x3f);
}
constexpr Bitboard::Square Move::to() const{
    return Bitboard::Square((data_ >> 6) & 0x3f);
}
constexpr Move Move::makeMove(Bitboard::Square from, Bitboard::Square to, MoveType type) {
    return Move(from, to, type);
}