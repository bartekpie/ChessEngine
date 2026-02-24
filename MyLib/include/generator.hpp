#include <cassert>
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
      inline void bitboardToMoves(Bitboard::Square from, Bitboard::bitboard b, MoveType m = standard) {
        while(b) {
          auto to = Bitboard::lsb(b);
          Bitboard::reset_bit(b, to);
          *this += Move::makeMove(from, to, m);
        }
      }

};
enum Pieces : uint8_t { Pawn = 0, Knight, Bishop, Queen, King};
enum directions : uint8_t { north=0, south, west, east, north_east, north_west, south_east, south_west, knight};
constexpr Bitboard::bitboard notAFile  = 0xfefefefefefefefeULL;
constexpr Bitboard::bitboard notABFile = 0xfcfcfcfcfcfcfcfcULL;
constexpr Bitboard::bitboard notHFile  = 0x7f7f7f7f7f7f7f7fULL;
constexpr Bitboard::bitboard notGHFile = 0x3f3f3f3f3f3f3f3fULL;

/*template<Pieces p>
void generate_moves(const Position& position, MoveList& list);*/

alignas(64) static constexpr std::array<std::array<Bitboard::bitboard,7>, 64> precompiled_directions = []()constexpr
{
  std::array<std::array<Bitboard::bitboard,7>, 64> table{};

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
	  Bitboard::bitboard N = 0ULL;
	  Bitboard::bitboard S = 0ULL;
	  Bitboard::bitboard E = 0ULL;
	  Bitboard::bitboard W = 0ULL;
	  for (int r{rank + 1}; r < 8; r++)
	  	table[square][north] |= 1ULL << (r * 8 + file);
    for (int r{rank - 1}; r >=0; r--)
			table[square][south] |= 1ULL << (r * 8 + file);
    for (int f{file+1}; f < 8; f++)
			table[square][east] |= 1ULL << (rank * 8 + f);
    for (int f{file-1}; f >=0; f--)
		  table[square][west] |= 1ULL << (rank * 8 + f);
    for (int r{rank + 1}, f{file +1}; r<8 && f < 8; r++, f++)
      table[square][north_east] |= 1ULL << (r * 8) + f ;
    for (int r{rank + 1}, f{file -1}; r<8 && f >= 0; r++, f--)
      table[square][north_west] |= 1ULL << (r * 8) + f ;
    for (int r{rank - 1}, f{file +1}; r>=0 && f < 8; r--, f++)
      table[square][south_east] |= 1ULL << (r * 8) + f ;
    for (int r{rank - 1}, f{file - 1}; r>=0 && f >= 0; r--, f--)
      table[square][south_west] |= 1ULL << (r * 8) + f ;
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
      for (int f{file-1}; f >= 0; f++)
			  moves[square] |= 1ULL << (r * 8 + f);
		  }
    }
    return moves;
}();

void generate_knight_moves(const Position& position, MoveList& list);
void generate_bishop_moves(const Position& position, MoveList& list);
//void generate_rook_moves(const Position& position, MoveList& list);
//void generate_queen_moves(const Position& position, MoveList& list);
//void generate_king_moves(const Position& position, MoveList& list);


