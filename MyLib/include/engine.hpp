#pragma once
#include "position.hpp"
#include "generator.hpp"
#include "move.hpp"
#include <iostream>
#include <atomic>
#include <limits>
#include <chrono>
#include <thread>


namespace Engine {

    enum playerType { min, max };
	class engine {
	private:
		Position position_{};
		MoveList moveList_{};
		std::atomic<Move> currBestMove_{};
		std::atomic<int> currDepth_{0};
		std::atomic<bool> isSearching_{false};
		std::atomic<std::int64_t> nodesSearched_{0};

		int evalulate();
		template <playerType player> int search(int depth, int alfa = std::numeric_limits<int>::min(), int beta = std::numeric_limits<int>::max());
	public:
		engine() = default;
		engine(Position position) : position_(position) {};
		engine(std::string fen_position) : position_(fen_position) {};
		Move bestMove(int depth, int alfa, int beta);
		Move iterativeDeepening(int maxDepth, int alfa, int beta);
		void printStatistics() const ;
	
	};
	constexpr int PieceEval[6] = {
    	100,  
    	320,  
    	330,  
    	500,  
    	900,  
    	20000  
	};
	constexpr int LocationEval[6][64] = {{
    	 0,  0,  0,  0,  0,  0,  0,  0,
    	50, 50, 50, 50, 50, 50, 50, 50,
    	10, 10, 20, 30, 30, 20, 10, 10,
    	 5,  5, 10, 25, 25, 10,  5,  5,
    	 0,  0,  0, 20, 20,  0,  0,  0,
    	 5, -5,-10,  0,  0,-10, -5,  5,
    	 5, 10, 10,-20,-20, 10, 10,  5,
    	 0,  0,  0,  0,  0,  0,  0,  0
	},
	{
	    -50,-40,-30,-30,-30,-30,-40,-50,
	    -40,-20,  0,  0,  0,  0,-20,-40,
	    -30,  0, 10, 15, 15, 10,  0,-30,
	    -30,  5, 15, 20, 20, 15,  5,-30,
	    -30,  0, 15, 20, 20, 15,  0,-30,
	    -30,  5, 10, 15, 15, 10,  5,-30,
	    -40,-20,  0,  5,  5,  0,-20,-40,
	    -50,-40,-30,-30,-30,-30,-40,-50
	},
	{
	    -20,-10,-10,-10,-10,-10,-10,-20,
	    -10,  0,  0,  0,  0,  0,  0,-10,
	    -10,  0,  5, 10, 10,  5,  0,-10,
	    -10,  5,  5, 10, 10,  5,  5,-10,
	    -10,  0, 10, 10, 10, 10,  0,-10,
	    -10, 10, 10, 10, 10, 10, 10,-10,
	    -10,  5,  0,  0,  0,  0,  5,-10,
	    -20,-10,-10,-10,-10,-10,-10,-20
	},
	{
	     0,  0,  0,  0,  0,  0,  0,  0,
	     5, 10, 10, 10, 10, 10, 10,  5,
	    -5,  0,  0,  0,  0,  0,  0, -5,
	    -5,  0,  0,  0,  0,  0,  0, -5,
	    -5,  0,  0,  0,  0,  0,  0, -5,
	    -5,  0,  0,  0,  0,  0,  0, -5,
	    -5,  0,  0,  0,  0,  0,  0, -5,
	     0,  0,  0,  5,  5,  0,  0,  0
	},
   {
	    -20,-10,-10, -5, -5,-10,-10,-20,
	    -10,  0,  0,  0,  0,  0,  0,-10,
	    -10,  0,  5,  5,  5,  5,  0,-10,
	     -5,  0,  5,  5,  5,  5,  0, -5,
	      0,  0,  5,  5,  5,  5,  0, -5,
	    -10,  5,  5,  5,  5,  5,  0,-10,
	    -10,  0,  5,  0,  0,  0,  0,-10,
	    -20,-10,-10, -5, -5,-10,-10,-20
	},
	{
	    -30,-40,-40,-50,-50,-40,-40,-30,
	    -30,-40,-40,-50,-50,-40,-40,-30,
	    -30,-40,-40,-50,-50,-40,-40,-30,
	    -30,-40,-40,-50,-50,-40,-40,-30,
	    -20,-30,-30,-40,-40,-30,-30,-20,
	    -10,-20,-20,-20,-20,-20,-20,-10,
	     20, 20,  0,  0,  0,  0, 20, 20,
	     20, 30, 10,  0,  0, 10, 30, 20
	}
};
 constexpr int mirrorSquare(int sq) {
    int rank = sq / 8;       // wyciągamy rząd (0–7)
    int file = sq % 8;       // wyciągamy kolumnę (0–7)
    int mirroredRank = 7 - rank;  // odbijamy rząd
    return mirroredRank * 8 + file;  // składamy z powrotem
}
}