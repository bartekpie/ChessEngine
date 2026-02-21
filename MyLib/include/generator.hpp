#include <cassert>
#include <array>
#include "bitboard.hpp"
#include "move.hpp"
constexpr size_t MaxMoves = 256;

struct MoveList {
    private:
      std::array<Move,256> data_ ;  
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

};
enum Pieces : uint8_t { Pawn = 0, Knight, Bishop, Queen, King};

template<Pieces>
void generate_moves(const Position& position, MoveList& list);

constexpr Bitboard::bitboard notAFile  = 0xfefefefefefefefeULL;
constexpr Bitboard::bitboard notABFile = 0xfcfcfcfcfcfcfcfcULL;
constexpr Bitboard::bitboard notHFile  = 0x7f7f7f7f7f7f7f7fULL;
constexpr Bitboard::bitboard notGHFile = 0x3f3f3f3f3f3f3f3fULL;

constexpr std::array<Bitboard::bitboard, 64> knight_precompiled = []() constexpr  
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
enum class Directions { North= 0, South, East, West, Size};
constexpr std::array<std::array<Bitboard::bitboard, 64>, int(Directions::Size)> rook_precompiled = []() constexpr {
    std::array<std::array<Bitboard::bitboard, 64>, int(Directions::Size)> moves {};
    for (auto square {0}; square < 64; square++) {
		int rank = square / 8;
		int file = square % 8;
		Bitboard::bitboard N = 0ULL;
		Bitboard::bitboard S = 0ULL;
		Bitboard::bitboard E = 0ULL;
		Bitboard::bitboard W = 0ULL;
		for (int r = rank + 1; r < 8; ++r)
			moves[int(Directions::North)][square] |= 1ULL << (r * 8 + file);
		for (int r = rank - 1; r >= 0; --r)
			moves[int(Directions::South)][square] |= 1ULL << (r * 8 + file);
		for (int f = file + 1; f < 8; ++f)
			moves[int(Directions::East)][square]  |= 1ULL << (rank * 8 + f);
		for (int f = file - 1; f >= 0; --f)
			moves[int(Directions::West)][square]  |= 1ULL << (rank * 8 + f);
		}
        return moves;
}();