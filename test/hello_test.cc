#include <gtest/gtest.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "chesstate.hpp"
#include "bitboard.hpp"
class ChessStateTest : public ::testing::Test {
protected:
	Chess::ChessState state;


};

TEST_F(ChessStateTest, defaultPositionCheck) {
	//kolory 
	EXPECT_TRUE(state.isWhiteMove()) << "Na pocz¹tku powinien by ruch bia³ych";
	EXPECT_EQ(state.getWhitePieces() & state.getBlackPieces(), 0ULL)
		<< "nie moze byc czêœci wspólnej ";
	//ustawienie figur
	EXPECT_EQ(state.getPieces(Chess::Pieces::bishops), (1ULL << 2) | (1ULL << 5) | (1ULL << 61) | (1ULL << 58));
	EXPECT_EQ(state.getPieces(Chess::Pieces::knights), (1ULL << 1) | (1ULL << 6) | (1ULL << 62) | (1ULL << 57));
	EXPECT_EQ(state.getPieces(Chess::Pieces::rooks), (1ULL << 0) | (1ULL << 7) | (1ULL << 63) | (1ULL << 56));
	EXPECT_EQ(state.getPieces(Chess::Pieces::queens), (1ULL << 3) | (1ULL << 59));
	EXPECT_EQ(state.getPieces(Chess::Pieces::kings), (1ULL << 4) | (1ULL << 60));
	
}


