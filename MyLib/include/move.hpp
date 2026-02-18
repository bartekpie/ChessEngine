#include <iostream>
#include "bitboard.hpp"

enum MoveType : uint16_t{
    standard, 
    passant = 1 << 12,
    promotion = 2 << 12,
    castle = 3 << 12
};
enum PromotionPiece : uint16_t{
    none, 
    knight = 1 << 14,
    bishop = 2 << 14,
    queen = 3 << 14
};
class move {
    private :
      uint16_t data_;
    public :
      move (Bitboard::Square from, Bitboard::Square to, MoveType type) : 
        data_((from | to << 6 | type)) {}
      MoveType type();
      Bitboard::Square from();
      Bitboard::Square to();
      static constexpr move makeMove(Bitboard::Square from, Bitboard::Square to, MoveType type = standard, PromotionPiece piece = none);

};
