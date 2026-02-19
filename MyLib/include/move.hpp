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
    passant = 1 << 12,
    promotion = 2 << 12,
    castle = 3 << 12
};
enum PromotionPiece : uint16_t{
    knight,
    bishop = 1 << 14,
    rook = 2 << 14,
    queen = 3 << 14
};
// Move class responsibilty is packing, and holding as much information about move as possible
// and extracting that information efficiently. 
class Move {
    private :
      uint16_t data_;
    public :
      constexpr Move(uint16_t move);
      constexpr Move(Bitboard::Square from, Bitboard::Square to, MoveType type = standard, PromotionPiece = knight); 
        
      constexpr MoveType type() const;
      constexpr PromotionPiece promotionType() const;
      constexpr Bitboard::Square from() const;
      constexpr Bitboard::Square to() const;
      constexpr uint16_t data() const;
      static constexpr Move makeMove(Bitboard::Square from, Bitboard::Square to, MoveType type = standard, PromotionPiece piece = knight);
      
};
constexpr Move::Move(uint16_t move) : 
  data_(move){}

constexpr Move::Move(Bitboard::Square from, Bitboard::Square to, MoveType type, PromotionPiece piece) : 
  data_(from | to << 6 | type | piece) {
  assert(from >= Bitboard::a1 && from <= Bitboard::h8);
  assert(to >= Bitboard::a1 && to <= Bitboard::h8);
  assert(type == MoveType::standard||(type >= MoveType::promotion && type <= MoveType::castle)); // values not one by one diifrent aproach than above nessesary
  assert(piece == PromotionPiece::knight||(piece >= PromotionPiece::bishop && piece <= PromotionPiece::queen));
}

constexpr MoveType Move::type() const{
    return( MoveType(data_  & 0x3000) );
}
constexpr uint16_t Move::data() const {
    return data_;
}
constexpr PromotionPiece Move::promotionType() const{
    return PromotionPiece(data_ & 0xC000);
}
constexpr Bitboard::Square Move::from() const{
    return Bitboard::Square(data_ & 0x3f);
}
constexpr Bitboard::Square Move::to() const{
    return Bitboard::Square((data_ >> 6) & 0x3f);
}
constexpr Move Move::makeMove(Bitboard::Square from, Bitboard::Square to, MoveType type, PromotionPiece piece) {
    return Move(from, to, type, piece);
}