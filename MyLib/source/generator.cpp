#include "generator.hpp"



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
  while (bishops) {
     Bitboard::Square square = Bitboard::lsb(bishops);
     Bitboard::reset_bit(bishops, square); 
     Bitboard::bitboard quiets {};
     Bitboard::bitboard captures {};
     for (int dir = int(north_east); dir < int(knight); dir++ ) {
         auto blockers = precompiled_directions[square][dir] & ~position.getEmptySpaces();
         if (blockers) {
          auto blocking_square = dir < int(south_east) ? Bitboard::lsb(blockers) : Bitboard::msb(blockers);
          auto reduced = precompiled_directions[square][dir] ^ precompiled_directions[blocking_square][dir];
          auto is_opponent_blocking = Bitboard::get_bit(position.getOpponents(), blocking_square);
          assert(Bitboard::get_bit(position.getOurs(),blocking_square) != is_opponent_blocking);
          auto moves = reduced | Bitboard::bitboard(is_opponent_blocking) << blocking_square ;
          captures |= moves & position.getOpponents();
          quiets   |= moves & position.getEmptySpaces();
       } else {
          quiets |= precompiled_directions[square][dir];
       }
    }
    list.bitboardToMoves(square, captures, capture);
    list.bitboardToMoves(square, quiets);
  }  
}

void generate_rook_moves(const Position& position, MoveList& list) {
  auto rooks = position.getOurs<PiecesType::rook>();
  while (rooks) {
     Bitboard::Square square = Bitboard::lsb(rooks);
     Bitboard::reset_bit(rooks, square); 
     Bitboard::bitboard quiets {};
     Bitboard::bitboard captures {};
     for (int dir = int(north); dir < int(north_east); dir++ ) {
         auto blockers = precompiled_directions[square][dir] & ~position.getEmptySpaces();
         if (blockers) {
          auto blocking_square = dir < int(south) ? Bitboard::lsb(blockers) : Bitboard::msb(blockers);
          auto reduced = precompiled_directions[square][dir] ^ precompiled_directions[blocking_square][dir];
          auto is_opponent_blocking = Bitboard::get_bit(position.getOpponents(), blocking_square);
          assert(Bitboard::get_bit(position.getOurs(),blocking_square) != is_opponent_blocking);
          auto moves = reduced | Bitboard::bitboard(is_opponent_blocking) << blocking_square ;
          captures |= moves & position.getOpponents();
          quiets   |= moves & position.getEmptySpaces();
       } else {
          quiets |= precompiled_directions[square][dir];
       }
    }
    list.bitboardToMoves(square, captures, capture);
    list.bitboardToMoves(square, quiets);
  }
}
 
   
