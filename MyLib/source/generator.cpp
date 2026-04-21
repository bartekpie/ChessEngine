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

Bitboard::Square get_blocking_square(precompiledType dir, Bitboard::bitboard b) {
   auto num_of_dir = int(dir);
   if ( (dir == north) || (dir == west) || (dir == north_east) || (dir == north_west)) {
      return Bitboard::lsb(b);
   } else {
      return Bitboard::msb(b);
   }
}
template <PiecesType piece> 
std::pair<Bitboard::bitboard, Bitboard::bitboard> generate_sliders_bb(const Position& position, Bitboard::Square square, Color color, bool a = false) {
   precompiledType starting_dir, ending_dir {};
   Bitboard::bitboard quiets, captures {};
   auto our_color = color;
   auto their_color = color == Color::white ? Color::black : Color::white;
   if constexpr (piece == PiecesType::rook)   {starting_dir = north; ending_dir = south;}
   if constexpr (piece == PiecesType::bishop) {starting_dir = north_east; ending_dir = south_west;}
   if constexpr (piece == PiecesType::queen)  {starting_dir = north; ending_dir = south_west;}
   auto occupancy = ~ position.getEmptySpaces();
   for (int dir = int(starting_dir); dir <= int(ending_dir); dir++ ) {
         auto blockers = precompiled_directions[square][dir] & occupancy;
         if (blockers) {
          auto blocking_square = get_blocking_square(dir, blockers);
          auto reduced = precompiled_directions[square][dir] ^ precompiled_directions[blocking_square][dir];
          auto is_opponent_blocking = Bitboard::get_bit(position.getPiecesByColor<their_color>(), blocking_square);
          if (a) is_opponent_blocking = true;
          assert(Bitboard::get_bit(position.getPiecesByColor<our_color>(),blocking_square) != is_opponent_blocking);
          auto moves = reduced | Bitboard::bitboard(is_opponent_blocking) << blocking_square ;
          
          captures |= moves & position.getOpponents();
          quiets   |= moves & position.getEmptySpaces();
       } else {
          quiets |= precompiled_directions[square][dir];
       }
   }
   return {quiets, captures};
}

void generate_bishop_moves(const Position& position, MoveList& list, Bitboard::bitboard limitedMoves = ~0ULL) {
  auto bishops = position.getOurs<PiecesType::bishop>();
  auto side_to_move = position.getSideToMove();
  while (bishops) {
     Bitboard::Square square = Bitboard::lsb(bishops);
     Bitboard::reset_bit(bishops, square); 
     auto [quiets, captures] = generate_sliders_bb<PiecesType::bishop>(position, square, side_to_move);
     list.bitboardToMoves(square, captures & limitedMoves, capture);
     list.bitboardToMoves(square, quiets & limitedMoves);
  }  
}

void generate_rook_moves(const Position& position, MoveList& list, Bitboard::bitboard limitedMoves = ~0ULL) {
  auto rooks = position.getOurs<PiecesType::rook>();
  auto side_to_move = position.getSideToMove();
  while (rooks) {
     Bitboard::Square square = Bitboard::lsb(rooks);
     Bitboard::reset_bit(rooks, square); 
     auto [quiets, captures] = generate_sliders_bb<PiecesType::rook>(position, square, side_to_move);
     list.bitboardToMoves(square, captures & limitedMoves, capture);
     list.bitboardToMoves(square, quiets & limitedMoves);
  }
}

void generate_queen_moves(const Position& position, MoveList& list, Bitboard::bitboard limitedMoves = ~0ULL) {
  auto queens = position.getOurs<PiecesType::queen>();
  auto side_to_move = position.getSideToMove();
  while (queens) {
     Bitboard::Square square = Bitboard::lsb(queens);
     Bitboard::reset_bit(queens, square); 
     auto [quiets, captures] = generate_sliders_bb<PiecesType::queen>(position, square, side_to_move);
     list.bitboardToMoves(square, captures & limitedMoves, capture);
     list.bitboardToMoves(square, quiets & limitedMoves);
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
template<verticalType dir> Bitboard::bitboard can_be_double_pushed() {
   if constexpr(dir == up)
      return uint64_t(0x0000000000FF0000);  // F in binary 1111
   else {
      return 0x0000FF0000000000;
   }
}
template<verticalType dir> Bitboard::bitboard left_en_passant(const Position& position, Bitboard::Square double_pushed) {
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
template<verticalType dir> Bitboard::bitboard right_en_passant(const Position& position, Bitboard::Square double_pushed) {
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
}
template<verticalType dir> 
void right_en_passant_to_moves(Bitboard::bitboard& to_bb, MoveList& list) { 
   if (to_bb) {
      auto to = int(Bitboard::lsb(to_bb));
      auto offset = 7;
      if constexpr (dir == up) offset = -7;
      auto from = to + offset;
      list += Move::makeMove(Bitboard::Square(from), Bitboard::Square(to), MoveType::passant);
   }
}
template<verticalType dir> 
void left_en_passant_to_moves(Bitboard::bitboard& to_bb, MoveList& list) { 
   if (to_bb) {
      auto to = int(Bitboard::lsb(to_bb));
      auto offset = 9;
      if constexpr (dir == up) offset = -9;
      auto from = to + offset;
      list += Move::makeMove(Bitboard::Square(from), Bitboard::Square(to), MoveType::passant);
   } 
}
template<verticalType type, int offset, MoveType mtype = standard> 
void from_push_to_moves(Bitboard::bitboard& push, MoveList& list) {
     Bitboard::Square from {};
     bool promotion = false;
     while(push) {
       auto to = Bitboard::lsb(push);
       if constexpr (type == up) { 
           from = Bitboard::Square(to - offset);
           if (to > 55) 
             promotion = true;
       }
       else {
          from = Bitboard::Square(to + offset);
          if (to < 8)
            promotion = true;
      }
      if (!promotion) 
         list += Move::makeMove(from, to, mtype);
      else {
         list += Move::makeMove(from, to, MoveType::promotionBishop);
         list += Move::makeMove(from, to, MoveType::promotionKnight);
         list += Move::makeMove(from, to, MoveType::promotionQueen);
         list += Move::makeMove(from, to, MoveType::promotionRook);
      }
      Bitboard::reset_bit(push, to);
      promotion = false;
    }
}
constexpr uint8_t push_offset = 8;
constexpr uint8_t double_push_offset = 16;
constexpr uint8_t attacks_short_offset = 7;
constexpr uint8_t attacks_long_offset = 9;
template<verticalType type>
std::pair<Bitboard::bitboard, Bitboard::bitboard> generate_pawn_capture_bb(const Position& position) {
   if constexpr (type == up){
      auto ours = position.getPiecesByColor<Color::white>();
      auto opponents = position.getPiecesByColor<Color::black>();
   } else {
      auto ours = position.getPiecesByColor<Color::black>();
      auto opponents = position.getPiecesByColor<Color::white>();
   }
   
   Bitboard::bitboard short_offset_attacks_bb  = short_offset_attacks<type>(ours) & opponents;
   Bitboard::bitboard long_offset_attacks_bb = long_offset_attacks<type>(ours) & opponents;

}
template<verticalType type>
void generate_pawn_moves_impl(const Position& position, MoveList& list) {
   Bitboard::bitboard push_bb          = push<type>(position.getOurs<PiecesType::pawn>()) & position.getEmptySpaces();
   Bitboard::bitboard double_push_bb   = push<type>(push_bb & can_be_double_pushed<type>()) & position.getEmptySpaces();
   auto [short_offset_attacks_bb, long_offset_attacks_bb] = generate_pawn_capture_bb<type>(position);
   auto double_pushed_move = position.getMoreInfo().doublePushedMove_;
   if (double_pushed_move != Bitboard::a1) {
      Bitboard::bitboard left_en_passant = left_en_passant<type>(position, double_pushed_move);
      Bitboard::bitboard right_en_passant = right_en_passant<type>(position, double_pushed_move);
      left_en_passant_to_moves<type>(left_en_passant, list);
      right_en_passant_to_moves<type>(right_en_passant, list);
   }
   from_push_to_moves<type, push_offset> (push_bb, list); 
   from_push_to_moves<type, double_push_offset> (double_push_bb, list); 
   from_push_to_moves<type, attacks_short_offset, capture> (short_offset_attacks_bb, list); 
   from_push_to_moves<type, attacks_long_offset, capture> (long_offset_attacks_bb, list); 
   
}
void generate_pawn_moves(const Position& position, MoveList& list) {
   position.getSideToMove() == Color::white 
     ? generate_pawn_moves_impl<up> (position, list)
     : generate_pawn_moves_impl<down> (position, list);
   
}
void generate_king_moves(const Position& position, MoveList& list, const MoveGenContext& cntx = MoveGenContext()) {
   Bitboard::Square our_king = Bitboard::lsb(position.getOurs<PiecesType::king>());
   Bitboard::Square their_king = Bitboard::lsb(position.getOpponents<PiecesType::king>());
   auto quiet = precompiled_directions[our_king][king] & position.getEmptySpaces() & ~precompiled_directions[their_king][king];
   auto captures = precompiled_directions[our_king][king] & position.getOpponents() & ~precompiled_directions[their_king][king];
   list.bitboardToMoves(our_king, quiet);
   list.bitboardToMoves(our_king, captures, capture);
}
/*template<verticalType dir>
void generate_castling_moves(const Position& position, MoveList& list) {
   if constexpr (dir == up) {
      bool whiteCanShortCastle = 
         whiteShortCastleRight &&
         Bitboard::count_bits(position.getEmptySpaces() & 0x60) == 2 &&
         enemy_attacking_king ;
      if (whiteCanShortCastle)
         list += Move::makeMove(Bitboard::e1, Bitboard::h1, MoveType::castle)
      bool whiteCanLongCastle = 
         whiteShortCastleRight && 
         Bitboard::count_bits(position.getEmptySpaces() & 0xE ) == 3 &&
         enemy_not_attacking_king ;
      if (whiteCanLongCastle)
        list += Move::makeMove(Bitboard::e1, Bitboard::a1, MoveType::castle)
   }
   else{
      bool blackCanShortCastle = blackShortCastleRight &&
      Bitboard::count_bits(position.getEmptySpaces() & 0x6000000000000000 ) == 2 &&
      enemy_attacking_king ;
      if (blackCanShortCastle)
        list += Move::makeMove(Bitboard::e8, Bitboard::h8, MoveType::castle)
      bool whiteCanLongCastle = 
      blackShortCastleRight && 
      Bitboard::count_bits(position.getEmptySpaces() & 0xE00000000000000 ) == 3 &&
      enemy_not_attacking_king ;
      if (blackCanShortCastle)
        list += Move::makeMove(Bitboard::e8, Bitboard::a8, MoveType::castle)
   }
  

}*/
/*void generate_all_moves(const Position& position, MoveList& list) {
   generate_pawn_moves  (position, list);
   generate_knight_moves(position, list);
   generate_bishop_moves(position, list);
   generate_rook_moves  (position, list);
   generate_queen_moves (position, list);
   generate_king_moves  (position, list);
}*/
struct MoveGenContext {
    Bitboard::bitboard checkers;     
    Bitboard::bitboard pinned;    
    Bitboard::bitboard opponent_attacks;
    Bitboard::bitboard check_mask; 
    int num_checks;
};
void find_pinned_and_attacking_pieces(const Position& position, MoveGenContext& ctx)
{
    auto king_sq  = Bitboard::lsb(position.getOurs<PiecesType::king>());
    auto ours     = position.getOurs();
    auto opponents= position.getOpponents();
    auto occupied = ~position.getEmptySpaces();

    auto rooks   = position.getOpponents<PiecesType::rook>();
    auto bishops = position.getOpponents<PiecesType::bishop>();
    auto queens  = position.getOpponents<PiecesType::queen>();

    for (int dir = int(north); dir < int(knight); dir++) {
        auto ray = precompiled_directions[king_sq][dir];
        auto blockers = ray & occupied;
        if (!blockers) continue;

        bool forward =
            (dir == north || dir == north_east ||
             dir == north_west || dir == east);

        auto first_sq = forward ? Bitboard::lsb(blockers)
                                : Bitboard::msb(blockers);

        
        if (Bitboard::get_bit(ours, first_sq)) {
            Bitboard::reset_bit(blockers, first_sq);

            if (!blockers) continue;

            auto second_sq = forward ? Bitboard::lsb(blockers)
                                     : Bitboard::msb(blockers);
            bool is_straight =
                (dir == north || dir == south || dir == east || dir == west);
            bool is_slider =
                is_straight
                ? (Bitboard::get_bit(rooks, second_sq) ||
                   Bitboard::get_bit(queens, second_sq))
                : (Bitboard::get_bit(bishops, second_sq) ||
                   Bitboard::get_bit(queens, second_sq));

            if (is_slider) {
                ctx.pinned |= (1ULL << first_sq);
            }
        }
        else {
            bool is_straight =
                (dir == north || dir == south || dir == east || dir == west);
            bool is_slider =
                is_straight
                ? (Bitboard::get_bit(rooks, first_sq) ||
                   Bitboard::get_bit(queens, first_sq))
                : (Bitboard::get_bit(bishops, first_sq) ||
                   Bitboard::get_bit(queens, first_sq));
            if (is_slider) {
                ctx.checkers |= (1ULL << first_sq);
                ctx.check_mask = ray ^ precompiled_directions[first_sq][dir];
            }
        }
    }
}
void find_opponents_possible_attacks(const Position& position,
                                     MoveGenContext& ctx)
{
    ctx.opponent_attacks = 0ULL;

    auto bishops = position.getOpponents<PiecesType::bishop>();
    auto rooks   = position.getOpponents<PiecesType::rook>();
    auto queens  = position.getOpponents<PiecesType::queen>();
    auto knights = position.getOpponents<PiecesType::knight>();
    auto pawns   = position.getOpponents<PiecesType::pawn>();

    auto opposite_color =
        position.getSideToMove() == Color::white ? Color::black : Color::white;
    auto vertical_type = 
        opposite_color == Color::white ? up : down;

    while (queens) {
        auto sq = Bitboard::lsb(queens);
        Bitboard::reset_bit(queens, sq);

        auto [q, c] =
            generate_sliders_bb<PiecesType::queen>(position, sq, opposite_color, true);

        ctx.opponent_attacks |= (q | c);
    }

    while (rooks) {
        auto sq = Bitboard::lsb(rooks);
        Bitboard::reset_bit(rooks, sq);

        auto [q, c] =
            generate_sliders_bb<PiecesType::rook>(position, sq, opposite_color, true);

        ctx.opponent_attacks |= (q | c);
    }

    while (bishops) {
        auto sq = Bitboard::lsb(bishops);
        Bitboard::reset_bit(bishops, sq);

        auto [q, c] =
            generate_sliders_bb<PiecesType::bishop>(position, sq, opposite_color, true);

        ctx.opponent_attacks |= (q | c);
    }

    while (knights) {
        auto sq = Bitboard::lsb(knights);
        Bitboard::reset_bit(knights, sq);

        ctx.opponent_attacks |= precompiled_directions[sq][knight];
    }
    
    while (pawns) {
        auto sq = Bitboard::lsb(pawns);
        Bitboard::reset_bit(pawns, sq);

        auto[l, r] = opposite_color == Color::white ? generate_pawn_capture_bb<up>(position) : generate_pawn_capture_bb<down>(position);
        ctx.opponent_attacks |= l | r;
            
    }
}
MoveGenContext build_context(const Position& position) {
    MoveGenContext ctx{};

    find_pinned_and_attacking_pieces(position, ctx);
    find_opponents_possible_attacks(position, ctx);
    ctx.num_checks = ctx.checkers ? Bitboard::count_bits(ctx.checkers) : 0;
    return ctx;
}
void generate_all_moves(const Position& position, MoveList& list) {
    auto ctx = build_context(position);

    if (ctx.num_checks == 2) {
        generate_king_moves(position, list, ctx);
        return;
    }

    if (ctx.num_checks == 1) {
        generate_pawn_moves  (position, list, ctx);
        generate_knight_moves(position, list, ctx);
        generate_bishop_moves(position, list, ctx);
        generate_rook_moves  (position, list, ctx);
        generate_queen_moves (position, list, ctx);
        generate_king_moves  (position, list, ctx);
        return;
    }

    generate_pawn_moves  (position, list, ctx);
    generate_knight_moves(position, list, ctx);
    generate_bishop_moves(position, list, ctx);
    generate_rook_moves  (position, list, ctx);
    generate_queen_moves (position, list, ctx);
    generate_king_moves  (position, list, ctx);
}