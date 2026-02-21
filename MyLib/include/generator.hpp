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