#pragma once
#include <cassert>
#include <utility>
#include <array>
#include "bitboard.hpp"
#include "position.hpp"
#include "move.hpp"
constexpr size_t MaxMoves = 256;

struct MoveList {
    private:
      std::array<Move,256> data_{};  
      uint16_t index_{0};   
    public:
      constexpr void operator+=(Move m) {
        assert(index_ < 256);
        data_[index_++] = m;
      }
      uint16_t size() const {
        return index_;
      }
      inline bool empty() {
        return (index_ == 0) ? true : false;
      }
      inline Move* begin() noexcept { 
        return data_.data(); 
      }

      inline Move* end() noexcept { 
        return data_.data() + index_; 
      }

      inline const Move* begin() const noexcept { 
        return data_.data(); 
      }
    
      inline const Move* end() const noexcept { 
        return data_.data() + index_; 
      }
      void reset() {index_ = 0;}
      inline void bitboardToMoves(Bitboard::Square from, Bitboard::bitboard b, MoveType m = standard) {
        while(b) {
          auto to = Bitboard::lsb(b);
          Bitboard::reset_bit(b, to);
          assert(to != from);
          *this += Move::makeMove(from, to, m);
        }
      }

};
enum precompiledType : uint8_t { north=0, west, south, east, north_east, north_west, south_east, south_west, knight, king};
constexpr Bitboard::bitboard notAFile  = 0xfefefefefefefefeULL;
constexpr Bitboard::bitboard notABFile = 0xfcfcfcfcfcfcfcfcULL;
constexpr Bitboard::bitboard notHFile  = 0x7f7f7f7f7f7f7f7fULL;
constexpr Bitboard::bitboard notGHFile = 0x3f3f3f3f3f3f3f3fULL;
constexpr Bitboard::bitboard noLimitation = ~0ULL;



alignas(64) static constexpr std::array<std::array<Bitboard::bitboard,10>, 64> precompiled_directions = []()constexpr
{
  std::array<std::array<Bitboard::bitboard,10>, 64> table{};

  //knights 
  Bitboard::bitboard current {};
  for (auto square{0}; square < 64; square++) {
    current = 1ULL << square;
    table[square][knight] |= (current << 17) & notAFile;
    table[square][knight] |= (current << 15) & notHFile;
    table[square][knight] |= (current << 10) & notABFile;
    table[square][knight] |= (current << 6)  & notGHFile;
    table[square][knight] |= (current >> 17) & notHFile;
    table[square][knight] |= (current >> 15) & notAFile;
    table[square][knight] |= (current >> 10) & notGHFile;
    table[square][knight] |= (current >> 6)  & notABFile;
  } 
  for (auto square {0}; square < 64; square++) {
	  int rank = square / 8;
	  int file = square % 8;
	  for (int r{rank + 1}; r < 8; r++)
	  	table[square][north] |= 1ULL << (r * 8 + file);
    for (int r{rank - 1}; r >=0; r--)
			table[square][south] |= 1ULL << (r * 8 + file);
    for (int f{file+1}; f < 8; f++)
			table[square][east] |= 1ULL << (rank * 8 + f);
    for (int f{file-1}; f >=0; f--)
		  table[square][west] |= 1ULL << (rank * 8 + f);
    for (int r{rank + 1}, f{file +1}; r<8 && f < 8; r++, f++)
      table[square][north_east] |= 1ULL << (r * 8 + f) ;
    for (int r{rank + 1}, f{file -1}; r<8 && f >= 0; r++, f--)
      table[square][north_west] |= 1ULL << (r * 8 + f) ;
    for (int r{rank - 1}, f{file +1}; r>=0 && f < 8; r--, f++)
      table[square][south_east] |= 1ULL << (r * 8 + f) ;
    for (int r{rank - 1}, f{file - 1}; r>=0 && f >= 0; r--, f--)
      table[square][south_west] |= 1ULL << (r * 8 + f) ;
	}
  std::array<std::pair<int,int>, 8> king_helper {{{0, 1}, {1,0}, {0, -1}, {-1, 0}, {1, 1}, {1, -1}, {-1, -1}, {-1, 1}}};
  for (int square{0}; square < 64; square++) { 
    int rank = square / 8;
    int file = square % 8;
    for (auto [rank_off, file_off] : king_helper) {
      auto new_rank = rank + rank_off;
      auto new_file = file + file_off;
      if (new_rank < 8 && new_rank >= 0 && new_file < 8 && new_file >=0 ) {
        table[square][king] |= 1ULL << (new_rank * 8 + new_file);
      }
    }
  }
  return table;
}();



alignas(64) static constexpr auto knight_precompiled = []() constexpr  
{
   std::array<Bitboard::bitboard, 64> moves {};
   Bitboard::bitboard current {};
   for (auto square{0}; square < 64; square++) {
     current = 1ULL << square;

     moves[square] |= (current << 17) & notAFile;
     moves[square] |= (current << 15) & notHFile;
     moves[square] |= (current << 10) & notABFile;
     moves[square] |= (current << 6)  & notGHFile;
     moves[square] |= (current >> 17) & notHFile;
     moves[square] |= (current >> 15) & notAFile;
     moves[square] |= (current >> 10) & notGHFile;
     moves[square] |= (current >> 6)  & notABFile;
   } 
   return moves;
}();
// table return all north possible moves
alignas(64) static constexpr auto north_precompiled = []() constexpr {
    std::array<Bitboard::bitboard, 64> moves {};
    for (auto square {0}; square < 64; square++) {
		int rank = square / 8;
		int file = square % 8;
		Bitboard::bitboard N = 0ULL;
		Bitboard::bitboard S = 0ULL;
		Bitboard::bitboard E = 0ULL;
		Bitboard::bitboard W = 0ULL;
		for (int r = rank + 1; r < 8; r++)
			moves[square] |= 1ULL << (r * 8 + file);
		}
    return moves;
}();
alignas(64) static constexpr auto south_precompiled = []() constexpr {
    std::array<Bitboard::bitboard, 64> moves {};
    for (auto square {0}; square < 64; square++) {
		int rank = square / 8;
		int file = square % 8;
		Bitboard::bitboard N = 0ULL;
		Bitboard::bitboard S = 0ULL;
		Bitboard::bitboard E = 0ULL;
		Bitboard::bitboard W = 0ULL;
		for (int r = rank - 1; r >=0; r--)
			moves[square] |= 1ULL << (r * 8 + file);
		}
    return moves;
}();
alignas(64) static constexpr auto east_precompiled = []() constexpr {
    std::array<Bitboard::bitboard, 64> moves {};
    for (auto square {0}; square < 64; square++) {
		int rank = square / 8;
		int file = square % 8;
		Bitboard::bitboard N = 0ULL;
		Bitboard::bitboard S = 0ULL;
		Bitboard::bitboard E = 0ULL;
		Bitboard::bitboard W = 0ULL;
		for (int f{file+1}; f < 8; f++)
			moves[square] |= 1ULL << (rank * 8 + f);
		}
    return moves;
}();
alignas(64) static constexpr auto west_precompiled = []() constexpr {
    std::array<Bitboard::bitboard, 64> moves {};
    for (auto square {0}; square < 64; square++) {
		int rank = square / 8;
		int file = square % 8;
		Bitboard::bitboard N = 0ULL;
		Bitboard::bitboard S = 0ULL;
		Bitboard::bitboard E = 0ULL;
		Bitboard::bitboard W = 0ULL;
		for (int f{file-1}; f >=0; f--)
			moves[square] |= 1ULL << (rank * 8 + f);
		}
    return moves;
}();
alignas(64) static constexpr auto north_east_precompiled = []() constexpr {
    std::array<Bitboard::bitboard, 64> moves {};
    for (auto square {0}; square < 64; square++) {
		int rank = square / 8;
		int file = square % 8;
		Bitboard::bitboard N = 0ULL;
		Bitboard::bitboard S = 0ULL;
		Bitboard::bitboard E = 0ULL;
		Bitboard::bitboard W = 0ULL;
    for (int r{rank+1}; r < 8; r++) {
      for (int f{file+1}; f < 8; f++)
			  moves[square] |= 1ULL << (r * 8 + f);
		  }
    }
    return moves;
}();
alignas(64) static constexpr auto north_west_precompiled = []() constexpr {
    std::array<Bitboard::bitboard, 64> moves {};
    for (auto square {0}; square < 64; square++) {
		int rank = square / 8;
		int file = square % 8;
		Bitboard::bitboard N = 0ULL;
		Bitboard::bitboard S = 0ULL;
		Bitboard::bitboard E = 0ULL;
		Bitboard::bitboard W = 0ULL;
    for (int r{rank+1}; r < 8; r++) {
      for (int f{file-1}; f >= 0; f--)
			  moves[square] |= 1ULL << (r * 8 + f);
		  }
    }
    return moves;
}();
alignas(64) static constexpr auto south_east_precompiled = []() constexpr {
    std::array<Bitboard::bitboard, 64> moves {};
    for (auto square {0}; square < 64; square++) {
		int rank = square / 8;
		int file = square % 8;
		Bitboard::bitboard N = 0ULL;
		Bitboard::bitboard S = 0ULL;
		Bitboard::bitboard E = 0ULL;
		Bitboard::bitboard W = 0ULL;
    for (int r{rank-1}; r >= 0; r--) {
      for (int f{file+1}; f < 8; f++)
			  moves[square] |= 1ULL << (r * 8 + f);
		  }
    }
    return moves;
}();
alignas(64) static constexpr auto south_west_precompiled = []() constexpr {
    std::array<Bitboard::bitboard, 64> moves {};
    for (auto square {0}; square < 64; square++) {
		  int rank = square / 8;
		  int file = square % 8;
		  Bitboard::bitboard N = 0ULL;
		  Bitboard::bitboard S = 0ULL;
		  Bitboard::bitboard E = 0ULL;
		  Bitboard::bitboard W = 0ULL;
      for (int r{rank-1}; r >= 0; r--) {
        for (int f{file-1}; f >= 0; f--)
		  	  moves[square] |= 1ULL << (r * 8 + f);
		    }
    }
    return moves;
}();
alignas(64) static constexpr auto king_precompiled = []() constexpr {
   std::array<Bitboard::bitboard, 64> moves {};
   for (int square{0}; square < 64; square++) { 
      if (int curr = square - 1; curr > 0 && curr < 64) moves[square] |= 1ULL << Bitboard::Square(curr); 
      if (int curr = square + 1; curr > 0 && curr < 64) moves[square] |= 1ULL << Bitboard::Square(curr);  
      if (int curr = square + 7; curr > 0 && curr < 64) moves[square] |= 1ULL << Bitboard::Square(curr); 
      if (int curr = square + 8; curr > 0 && curr < 64) moves[square] |= 1ULL << Bitboard::Square(curr); 
      if (int curr = square + 9; curr > 0 && curr < 64) moves[square] |= 1ULL << Bitboard::Square(curr); 
      if (int curr = square - 7; curr > 0 && curr < 64) moves[square] |= 1ULL << Bitboard::Square(curr); 
      if (int curr = square - 8; curr > 0 && curr < 64) moves[square] |= 1ULL << Bitboard::Square(curr); 
      if (int curr = square - 9; curr > 0 && curr < 64) moves[square] |= 1ULL << Bitboard::Square(curr); 
   }
   return moves;
}();
enum verticalType {up =0, down};
enum slideType : bool {includeOurPieces = 0, legal = 1};
struct MoveGenContext {
    Bitboard::bitboard checkers{};     
    Bitboard::bitboard pinned{0ULL};    
    Bitboard::bitboard opponent_attacks{};
    Bitboard::bitboard check_mask{}; 
    Bitboard::bitboard pinMask[64]{};
    int num_checks{0};
};
template <PiecesType piece, Color color> std::pair<Bitboard::bitboard, Bitboard::bitboard> generate_sliders_bb(const Position& position, Bitboard::Square square, bool is_legal = legal);
template<verticalType dir> Bitboard::bitboard push(Bitboard::bitboard b);
template<verticalType dir> Bitboard::bitboard double_push(Bitboard::bitboard b);
template<verticalType dir> inline Bitboard::bitboard short_offset_attacks(Bitboard::bitboard b);
template<verticalType dir> inline Bitboard::bitboard long_offset_attacks(Bitboard::bitboard b);
template<verticalType dir> Bitboard::bitboard startingPawnsRow();
template<verticalType dir> Bitboard::bitboard left_en_passant(const Position& position, Bitboard::Square double_pushed);
template<verticalType dir> Bitboard::bitboard right_en_passant(const Position& position, Bitboard::Square double_pushed);
template<verticalType dir> void left_en_passant_to_moves(Bitboard::bitboard& to_bb, MoveList& list);
template<verticalType dir> void right_en_passant_to_moves(Bitboard::bitboard& to_bb, MoveList& list);
template<verticalType type, int offset, MoveType mtype> void from_push_to_moves(Bitboard::bitboard& push, MoveList& list);
//template<verticalType dir> void generate_castling_moves(const Position& position, MoveList& list);
template<verticalType type> void generate_pawn_moves_impl(const Position& position, MoveList& list);
 
void generate_pawn_moves  (const Position& position, MoveList& list, const Bitboard::bitboard& limitedMoves = noLimitation);
void generate_knight_moves(const Position& position, MoveList& list, const Bitboard::bitboard& limitedMoves = noLimitation);
void generate_bishop_moves(const Position& position, MoveList& list, const Bitboard::bitboard& limitedMoves = noLimitation);
void generate_rook_moves  (const Position& position, MoveList& list, const Bitboard::bitboard& limitedMoves = noLimitation);
void generate_queen_moves (const Position& position, MoveList& list, const Bitboard::bitboard& limitedMoves = noLimitation);
void generate_king_moves  (const Position& position, MoveList& list, const Bitboard::bitboard& limitedMovesKing = noLimitation);


