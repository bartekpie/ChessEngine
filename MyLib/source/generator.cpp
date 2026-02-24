#include "generator.hpp"

template<Pieces p> 
void generate_moves(const Position& position, MoveList& list) {
  if constexpr (p == Pieces::Knight) {
    auto b = position.getOurs<PiecesType::knight>();
    while(b) {
      auto current = Bitboard::lsb(b);
      list.bitboardToMoves(current, knight_precompiled[current] & ~ position.getOurs() & ~ position.getOpponents());
      list.bitboardToMoves(current, knight_precompiled[current] & ~ position.getOurs() & position.getOpponents(), capture);
      Bitboard::reset_bit(b, current);
    }
  }
  
}

void generate_knight_moves(const Position& position, MoveList& list) {
    auto b = position.getOurs<PiecesType::knight>();
    while(b) {
      auto current = Bitboard::lsb(b);
      list.bitboardToMoves(current, knight_precompiled[current] & ~ position.getOurs() & ~ position.getOpponents());
      list.bitboardToMoves(current, knight_precompiled[current] & ~ position.getOurs() & position.getOpponents(), capture);
      Bitboard::reset_bit(b, current);
    }
}
void generate_bishop_moves(const Position& position, MoveList& list) {
  auto bishops = position.getOurs<PiecesType::bishop>();
  while (bishops) 
     Bitboard::Square square = Bitboard::lsb(bishops);
     Bitboard::reset_bit(bishops, square); 
     Bitboard::bitboard quiets {};
     Bitboard::bitboard captures {};
     for (int dir = int(north_east); dir < int(knight); dir++ ) {
         auto blockers = north_east_precompiled[square] & ~position.getOpponents();
         if (blockers) {
          auto blocking_square = Bitboard::lsb(blockers);
          auto reduced = north_east_precompiled[square] ^ north_east_precompiled[blocking_square];
          auto is_opponent_blocking = Bitboard::get_bit(position.getOpponents(), blocking_square);
          assert(Bitboard::get_bit(position.getOpponents(),blocking_square) != is_opponent_blocking);
          auto moves = reduced | is_opponent_blocking << blocking_square ;
          captures |= moves & position.getOpponents();
          quiets   |= moves & position.getEmptySpaces();
       } else {
          quiets |= north_east_precompiled[square];
       }
     }
     
  }
   
}