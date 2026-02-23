#include "bitboard.hpp"
#include <array>
enum class Pieces {
    white_pawn = 0, white_knight, white_bishop, white_queen, white_king,
    black_pawn, black_knight, black_bishop, black_queen, black_king, size_of_pieces
};
enum class Color {
    black = 0, white
};
class Position { 
    private:
      std::array<Bitboard::bitboard, int(Pieces::size_of_pieces)> board_;
      Color sideToMove ;

}