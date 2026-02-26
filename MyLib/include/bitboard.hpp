#pragma once
#include <cstdint>
#include <iostream>
#include <cassert>
#if defined(_MSC_VER)
    #include <intrin.h>
#endif
namespace Bitboard {

  using bitboard = uint64_t;

  // using enum instead of enum class for clarity 
  // implicit conversion is better in this case instead of using static_cast 
  enum Square : uint8_t {
      a1, b1, c1, d1, e1, f1, g1, h1,
      a2, b2, c2, d2, e2, f2, g2, h2,
      a3, b3, c3, d3, e3, f3, g3, h3,
      a4, b4, c4, d4, e4, f4, g4, h4,
      a5, b5, c5, d5, e5, f5, g5, h5,
      a6, b6, c6, d6, e6, f6, g6, h6,
      a7, b7, c7, d7, e7, f7, g7, h7,
      a8, b8, c8, d8, e8, f8, g8, h8
  };
  // inline functions for maximum efficency 

  inline Square lsb(bitboard b) {
    assert(b);
    #if defined(_MSC_VER)
      unsigned long index;
      _BitScanForward64(&index, b);
      return static_cast<Square>(index);
    
    #elif defined(__GNUC__)
      return Square(__builtin_ctzll(b));
    #endif
  }

  inline Square msb(bitboard b) {
    assert(b);
    #if defined(_MSC_VER)
      unsigned long index;
      _BitScanReverse64(&index, b);
      return static_cast<Square>(index);
    
    #elif defined(__GNUC__)
      return Square(__builtin_clzll(b));
    #endif
  }

  inline Square count_bits(bitboard b) {
    assert(b);
    #if defined(_MSC_VER)
      unsigned long index;
      return static_cast<Square>(__popcnt64(b));
    
    #elif defined(__GNUC__)
      return Square(__builtin_popcountll(b));
    #endif
  }
  
  inline void set_bit(bitboard& b, Square index)
  {
      b |= (1ULL << index);
  }

  inline void reset_bit(bitboard& b, Square index)
  {
      b &= ~(1ULL << index);
  }

  inline bool get_bit(bitboard& b, Square index) {
  
      return (b >> index) & 1ULL; 
  }
  inline bool get_bit(bitboard b, Square index) {
  
      return (b >> index) & 1ULL; 
  }

  inline void print_bitboard(const bitboard& b){
     for (int rank = 7; rank >= 0; --rank) {
        std::cout << rank + 1 << " | ";
        for (int file = 0; file < 8; ++file) {
            Square sq = (Square)(rank * 8 + file);
            if (get_bit(b, sq)) {
                std::cout << "X ";
            }
            else {
                std::cout << ". ";
            }
        }
        std::cout << std::endl;
    }
    std::cout << "   -----------------" << std::endl;
    std::cout << "    A B C D E F G H" << std::endl << std::endl;
  }

}




