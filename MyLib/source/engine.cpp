#include "engine.hpp"


namespace Engine
{


    int engine::evalulate()
    {
        int score = 0;

        for (int pt = 0; pt < 6; pt++) {
            {
                Bitboard::bitboard curr_bb = position_.getPiecesByColor<Color::white>(static_cast<PiecesType>(pt)); 
                while (curr_bb) {
                    auto sq = Bitboard::lsb(curr_bb);
                    Bitboard::reset_bit(curr_bb, sq);                          
                    score += PieceEval[pt];                
                    score += LocationEval[pt][int(sq)];             
                } 
            }


            {
                Bitboard::bitboard curr_bb = position_.getPiecesByColor<Color::black>(static_cast<PiecesType>(pt)); 
                while (curr_bb) {
                    auto sq = Bitboard::lsb(curr_bb); 
                    Bitboard::reset_bit(curr_bb, sq);  

                    score -= PieceEval[pt];                
                    score -= LocationEval[pt][mirrorSquare(sq)];             
                } 
            }
        }

        return (this->position_.getSideToMove() == Color::white ? score : -score);
    }
    
    template <playerType player>
    int engine::search(int depth, int alfa, int beta)
    {
        nodesSearched_++;
        MoveList move_list {};
        GameStatus status = generate_all_moves(position_, move_list);
        if (status == GameStatus::checkmate) {
            return (player == max) ? std::numeric_limits<int>::min() : std::numeric_limits<int>::max();
        }
        if (status == GameStatus::pat) {
            return 0;
        }
        if (depth == 0)
            return evalulate();
        if constexpr (player == max)
        {
            int max = std::numeric_limits<int>::min();
            for (auto move : move_list) {
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
            return max;
        }
        else
        {
            int min = std::numeric_limits<int>::max();
            for (auto move : move_list) {
                position_.simulate_move(move);
                int actual = search<max>(depth - 1, alfa, beta);
                position_.undo_move();
                if (actual < min)
                    min = actual;
                if (actual < beta)
                    beta = actual;
                if (alfa >= beta)
                    break;
            }
            return min;
        }

    }

    Move engine::bestMove(int depth, int alfa, int beta)
    {
       nodesSearched_ = 0;
       MoveList move_list {};
       generate_all_moves(position_, move_list);

        Move bestMove {};
        if (position_.getSideToMove() == Color::white)
        {
            int max = std::numeric_limits<int>::min();
            for (auto move : move_list) {
                position_.simulate_move(move);
                int actual = search<min>(depth - 1, alfa, beta);
                position_.undo_move();
                if (actual > max){
                    max = actual;
                    bestMove = move;
                }
                if (actual > alfa)
                    alfa = actual;
                if (alfa >= beta)
                    break;
            }
            
        }
        else
        {
            int min = std::numeric_limits<int>::max();
            for (auto move : moveList_) {
                position_.simulate_move(move);
                int actual = search<max>(depth - 1, alfa, beta);
                position_.undo_move();
                if (actual < min){
                    min = actual;
                    bestMove = move;
                }
                if (actual < beta)
                    beta = actual;
                if (alfa >= beta)
                    break;
            }
            
        }
        return bestMove;

    }
    Move engine::iterativeDeepening(int maxDepth, int alfa, int beta)
    {
        isSearching_.store(true);
        for ( int curr_depth = 1 ; curr_depth <= maxDepth; curr_depth++) {
            currDepth_.store(curr_depth);
            currBestMove_ = this->bestMove(curr_depth, -1000, 1000);
            std::cout<<"Best move found at dept" << currDepth_ <<": " << currBestMove_.load().toString() <<std::endl;
        }
        isSearching_.store(false);
        return currBestMove_;
    }
    void engine::printStatistics() const
    {
        uint64_t lastNodes = 0;
        auto lastTime = std::chrono::steady_clock::now();

        while (isSearching_.load()) {
            std::this_thread::sleep_for(std::chrono::seconds(10));

            uint64_t currentNodes = nodesSearched_.load();
            auto now = std::chrono::steady_clock::now();

            double elapsed = std::chrono::duration<double>(now - lastTime).count();
            uint64_t diff = currentNodes - lastNodes;

            uint64_t nps = diff / elapsed;
            std::cout <<"Currently on depth: " << currDepth_.load() << std::endl;
            std::cout << "Current best move: " << currBestMove_.load().toString() << std::endl;
            std::cout << "Info nodes " << currentNodes << " current pace : " << nps << "nodes/second" <<std::endl;

            lastNodes = currentNodes;
            lastTime = now;
        }
    }
    
};