#include "move.hpp"

constexpr Move::Move(uint16_t move) : 
  data_(move){}

constexpr Move::Move(Bitboard::Square from, Bitboard::Square to, MoveType type = standard, PromotionPiece piece = none) : 
  data_(from | to << 6 | type | piece) {}

MoveType Move::type() const{
    return( MoveType(data_  & 0x3000) );
}
PromotionPiece Move::promotionType() const{
    return PromotionPiece(data_ & 0xC000)
}
Bitboard::Square Move::from() const{
    return Bitboard::Square(data_ & 0x3f);
}
Bitboard::Square Move::to() const{
    return Bitboard::Square((data_ >> 6) & 0x3f);
}
constexpr Move Move::makeMove(Bitboard::Square from, Bitboard::Square to, MoveType type = standard, PromotionPiece piece = none) {
    return Move(from, to, type, piece);
}