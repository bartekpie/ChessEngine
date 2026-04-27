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
		std::atomic<bool> isSearching_{false};
		std::atomic<std::int64_t> nodesSearched_{0};
	public:
		int evalulate();
		template <playerType player> int search(int depth, int alfa = std::numeric_limits<int>(min), int beta = std::numeric_limits<int>(max));
		Move bestMove(int depth, int alfa, int beta);
		void printStatistics() const ;
	
	};

}