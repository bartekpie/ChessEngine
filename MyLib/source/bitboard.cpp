#include "bitboard.hpp"
enum Square {
    a1, b1, c1, d1, e1, f1, g1, h1,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a8, b8, c8, d8, e8, f8, g8, h8
};
void setBit(U64& bitboard, int bit)
{
    bitboard = bitboard | (1ULL << bit);
}
void resetBit(U64& bitboard, int bit)
{
    bitboard = bitboard & ~(1ULL << bit);
}
bool getBit(U64 bitboard, int sq) {

    return (bitboard >> sq) & 1ULL; // przesuwa bitboard w lewo i powrównuje tylko pierwszy bit
}
int resetLSB(U64& bitboard)
{
    int a = std::countr_zero(bitboard);
    resetBit(bitboard, a);
    return a;
}
int howmanyBits(U64 bitboard)
{
    int i = 0;
    while (bitboard != 0ULL)
    {
        resetBit(bitboard, std::countr_zero(bitboard));
        i++;
    }
    return i;
}
void printBitboard(U64 bitboard)
{
    for (int rank = 7; rank >= 0; --rank) {
        std::cout << rank + 1 << " | ";
        for (int file = 0; file < 8; ++file) {
            Square sq = (Square)(rank * 8 + file);
            if (getBit(bitboard, sq)) {
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

