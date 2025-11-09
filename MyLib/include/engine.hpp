#pragma once

#include "chesstate.hpp"
#include "bitboard.hpp"
#include "const.hpp"

namespace Engine {

	class engine
	{
	public:
		engine(Chess::ChessState& startingstate) : state(startingstate) {}
		Chess::MoveList movelist[max_depth];
		Chess::MoveList checkhelper;
		int evalulate();
		int search(int depth, int alfa, int beta);
		Chess::Move bestMove(int depth, int alfa, int beta);
	private:
		Chess::ChessState& state;
	};

}