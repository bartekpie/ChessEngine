#include <gtest/gtest.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "chesstate.hpp"

// Demonstrate some basic assertions.
TEST(HelloTest, SimualateMove) {
	Chess::ChessState chessState;
	U64 begginPawns = chessState.getPieces(Chess::Pieces::pawns);
	U64 begginBishops = chessState.getPieces(Chess::Pieces::bishops);
	U64 begginKnights = chessState.getPieces(Chess::Pieces::knights);
	U64 begginRooks = chessState.getPieces(Chess::Pieces::rooks);
	U64 begginQueen = chessState.getPieces(Chess::Pieces::queens);
	U64 begginKing = chessState.getPieces(Chess::Pieces::kings);

	std::srand(std::time(nullptr));
	int a = 100;
	for (int z = 0; z < 3; z++) {
		for (int i = 0; i < a; i++)
		{
			if (chessState.generateLegalMoves() == 0)
			{
				Chess::MoveList local = chessState.getMoves();
				int los = std::rand() % (local.count + 1);
				std::cout << los;
				chessState.Simulate_Move(local.moves[los]);
			}
			else {
				a = i - 1;
				std::cout << a;
				break;
			}

		}
		for (int i = 0; i < a; i++)
		{
			chessState.undo_Move();
		}

		U64 endPawns = chessState.getPieces(Chess::Pieces::pawns);
		U64 endBishops = chessState.getPieces(Chess::Pieces::bishops);
		U64 endKnights = chessState.getPieces(Chess::Pieces::knights);
		U64 endRooks = chessState.getPieces(Chess::Pieces::rooks);
		U64 endQueen = chessState.getPieces(Chess::Pieces::queens);
		U64 endKing = chessState.getPieces(Chess::Pieces::kings);
		EXPECT_EQ(begginPawns, endPawns);
		EXPECT_EQ(begginBishops, endBishops);
		EXPECT_EQ(begginKnights, endKnights);
		EXPECT_EQ(begginRooks, endRooks);
		EXPECT_EQ(begginQueen, endQueen);
		EXPECT_EQ(begginKing, endKing);
	}
	
  
}
