#pragma once
#include "position.hpp"
#include "generator.hpp"
#include "move.hpp"
#include <atomic>
#include <limits>
#include <chrono>


namespace Engine {
    enum playerType { min, max };
	class engine
	{
	public:
		int evalulate();
		template <playerType player> int search(int depth, int alfa, int beta);
		Move bestMove(int depth, int alfa, int beta);
	private:
		Position position_;
		MoveList moveList_;
		std::atomic<bool> stopFlag_{false};
		std::int64_t nodesSearched_{0};
	};

}