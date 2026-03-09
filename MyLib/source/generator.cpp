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

void generate_queen_moves(const Position& position, MoveList& list) {
  auto queens = position.getOurs<PiecesType::queen>();
  while (queens) {
     Bitboard::Square square = Bitboard::lsb(queens);
     Bitboard::reset_bit(queens, square); 
     Bitboard::bitboard quiets {};
     Bitboard::bitboard captures {};
     for (int dir = int(north); dir < int(knight); dir++ ) {
         auto blockers = precompiled_directions[square][dir] & ~position.getEmptySpaces();
         if (blockers) {
          auto blocking_square = (dir < int(south) || dir == north_east || dir == north_west ) ? Bitboard::lsb(blockers) : Bitboard::msb(blockers);
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

template<verticalType dir> inline Bitboard::bitboard push(Bitboard::bitboard b) {
   if constexpr(dir == up)
     return b << 8;
   else {
      return b >> 8;
   }
}
template<verticalType dir> inline Bitboard::bitboard double_push(Bitboard::bitboard b) {
   if constexpr(dir == up)
      return b << 16;
   else {
      return b >> 16;
   }
}
template<verticalType dir> inline Bitboard::bitboard short_offset_attacks(Bitboard::bitboard b) {
   if constexpr(dir == up)
     return b << 7;
   else {
      return b >> 7;
   }
}
template<verticalType dir> inline Bitboard::bitboard long_offset_attacks(Bitboard::bitboard b) {
   if constexpr(dir == up)
      return b << 9;
   else {
      return b >> 9;
   }
}
template<verticalType dir> Bitboard::bitboard startingPawnsRow() {
   if constexpr(dir == up)
      return uint64_t(0x000000000000FF00);  // F in binary 1111
   else {
      return 0x00FF000000000000;
   }
}
/*template<verticalType dir> Bitboard::bitboard left_en_passant(Bitboard::Square double_pushed) {
   if constexpr (dir == up) {
     if(double_pushed >> 1 & position.getOurs()) {
       return double_pushed << 8 & position.getEmptySpaces();
     }
   }    
   else {
      if(double_pushed << 1 & position.getOurs()) {
       return double_pushed >> 8 & position.getEmptySpaces();
     }
   }
}
template<verticalType dir> Bitboard::bitboard right_en_passant(Bitboard::Square double_pushed) {
   if constexpr (dir == up) {
     if(double_pushed << 1 & position.getOurs()) {
       return double_pushed << 8 & position.getEmptySpaces();
     }
   }    
   else {
      if(double_pushed >> 1 & position.getOurs()) {
       return double_pushed >> 8 & position.getEmptySpaces();
     }
   }
}*/
template<verticalType type, int offset, MoveType mtype = standard> 
void from_push_to_moves(Bitboard::bitboard& push, MoveList& list) {
     Bitboard::Square from {};
     while(push) {
       auto to = Bitboard::lsb(push);
       if constexpr(type == up)  from = Bitboard::Square(to - offset);
       else { from = Bitboard::Square(to + offset);}
       list += Move::makeMove(from, to, mtype);
       Bitboard::reset_bit(push, to);
    }
}

template<verticalType type>
void generate_pawn_moves_impl(const Position& position, MoveList& list) {
   Bitboard::bitboard push_bb          = push<type>(position.getOurs<PiecesType::pawn>()) & position.getEmptySpaces();
   Bitboard::bitboard double_push_bb   = push<type>(push_bb & startingPawnsRow<type>()) & position.getEmptySpaces();
   Bitboard::bitboard short_offset_attacks_bb  = short_offset_attacks<type>(position.getOurs()) & position.getOpponents();
   Bitboard::bitboard long_offset_attacks_bb = long_offset_attacks<type>(position.getOurs()) & position.getOpponents();
   /*if (position.getDoublePushedMove()) {
      Bitboard::bitboard left_en_passant = left_en_passant<type>(position.getDoublePushedMove());
      Bitboard::bitboard right_en_passant = right_en_passant<type>(position.getDoublePushedMove());
   }*/
   from_push_to_moves<type, 8> (push_bb, list); 
   from_push_to_moves<type, 16> (double_push_bb, list); 
   from_push_to_moves<type, 7, capture> (short_offset_attacks_bb, list); 
   from_push_to_moves<type, 9, capture> (long_offset_attacks_bb, list); 
}
void generate_pawn_moves(const Position& position, MoveList& list) {
   position.getSideToMove() == Color::white 
     ? generate_pawn_moves_impl<up> (position, list)
     : generate_pawn_moves_impl<down> (position, list);
   
}