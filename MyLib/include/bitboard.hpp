#pragma once
#include"types.hpp"
#include<iostream>
#include <bit>
typedef unsigned long long U64;
extern enum Square;
void setBit(U64& bitboard, int bit);

void resetBit(U64& bitboard, int bit);

bool getBit(U64 bitboard, int sq);
int resetLSB(U64& bitboard);
int howmanyBits(U64 bitboard);

void printBitboard(U64 bitboard);
