#include "engine.hpp"


namespace Engine
{


    int engine::evalulate()
    {
        
         return 0;
    }
    
    template <playerType player>
    int engine::search(int depth, int alfa, int beta)
    {
        nodesSearched_++;
        if (stopFlag_) return 0;
        if (depth == 0)
            return evalulate();
        MoveList move_list {};
        generate_all_moves(position_, move_list); 
        if constexpr (player == max)
        {
            int max = std::numeric_limits<int>::max();
            for (auto move : move_list_) {
                position_.makeMove(move);
                int actual = search<min>(depth - 1, alfa, beta);
                position_.undoMove(move);
                if (actual > max)
                    max = actual;
                if (actual > alfa)
                    alfa = actual;
                if (alfa >= beta)
                    break;
            }
            return max;
        }
        else
        {
            int min = std::numeric_limits<int>::min();
            for (auto move : moveList_) {
                position_.simulateMove(move);
                int actual = search<max>(depth - 1, alfa, beta);
                position_.undoMove(move);
                if (actual < min)
                    min = actual;
                if (actual < beta)
                    alfa = actual;
                if (alfa >= beta)
                    break;
            }
            return min;
        }

    }

    Move engine::bestMove(int depth, int alfa, int beta)
    {
       nodesSearched_ = 0;
       auto start = std::chrono::high_resolution_clock::now();

       generate_all_moves(position_, moveList_);

        Move bestMove {};
        if (position_.getSideToMove() == Color::white)
        {
            int max = std::numeric_limits<int>::min();
            for (auto move : moveList_) {
                position_.simulate_move(move);
                int actual = search<min>(depth - 1, alfa, beta);
                position_.undo_move();
                if (actual > max)
                    max = actual;
                if (actual > alfa)
                    alfa = actual;
                if (alfa >= beta)
                    break;
            }
            return bestMove;
        }
        else
        {
            int min = std::numeric_limits<int>::max();
            for (auto move : moveList_) {
                position_.simulate_move(move);
                int actual = search<max>(depth - 1, alfa, beta);
                position_.undo_move();
                if (actual < min)
                    min = actual;
                if (actual < beta)
                    alfa = actual;
                if (alfa >= beta)
                    break;
            }
            
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        double seconds = elapsed.count();
        std::cout <<"Nodes searched: " << nodesSearched_ << " in " << seconds << " seconds. NPS: " << nodesSearched_ / seconds << std::endl;
        return bestMove;

    }
}