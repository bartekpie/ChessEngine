#include <gtest/gtest.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "chesstate.hpp"
#include "bitboard.hpp"
#include "game.hpp"
class ChessStateTest : public ::testing::Test {
protected:
	Chess::ChessState state;


};
class SaveMoveTest : public ::testing::Test {
protected:
	Game::SaveMove saver_t;


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

TEST_F(SaveMoveTest, strTointCheck) {
	//kolory 
	std::string a1 = "a1";
	EXPECT_EQ(saver_t.strToint(a1), 0);
	std::string h8 = "h8";
	EXPECT_EQ(saver_t.strToint(h8), 63);
	std::string c3 = "c3";
	EXPECT_EQ(saver_t.strToint(c3), 18);

}
TEST_F(SaveMoveTest, fromIndex) {

	EXPECT_EQ(saver_t.fromIndex(0), "a1");
	EXPECT_EQ(saver_t.fromIndex(63), "h8");
	EXPECT_EQ(saver_t.fromIndex(18), "c3");

}
