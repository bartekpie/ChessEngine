#include "bitboard.hpp"
enum Square {
    A1, B1, C1, D1, E1, F1, G1, H1,
    A2, B2, C2, D2, E2, F2, G2, H2,
    A3, B3, C3, D3, E3, F3, G3, H3,
    A4, B4, C4, D4, E4, F4, G4, H4,
    A5, B5, C5, D5, E5, F5, G5, H5,
    A6, B6, C6, D6, E6, F6, G6, H6,
    A7, B7, C7, D7, E7, F7, G7, H7,
    A8, B8, C8, D8, E8, F8, G8, H8

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

