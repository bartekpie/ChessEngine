
#include "engine.hpp"

namespace Engine
{


    int engine::evalulate()
    {
        U64 center = (1ULL << 27) | (1ULL << 28) | (1ULL << 35) | (1ULL << 36);
        U64 aroundcenter = (1ULL << 18) | (1ULL << 19) | (1ULL << 20) | (1ULL << 21) | (1ULL << 26) | (1ULL << 34) | (1ULL << 42) | (1ULL << 43) | (1ULL << 44) | (1ULL << 45) | (1ULL << 29) | (1ULL << 37) | (1ULL << 45);
        int score = 0;
        // ogolne 
        for (int p = 0; p < Chess::sizePieces; p++) {
            U64 bb = state.getPieces(p);
            while (bb) {
                int sq = resetLSB(bb);
                if (getBit(state.getWhitePieces(), sq))
                    score += pieceValue[p];
                if (getBit(state.getBlackPieces(), sq))
                    score -= pieceValue[p];
            }
        }
        score += 10 * howmanyBits(center & state.getWhitePieces());
        score -= 10 * howmanyBits(center & state.getBlackPieces());
        //piony 

        score += 30 * howmanyBits(center & state.getWhitePieces() & state.getPieces(Chess::pawns));
        score -= 30 * howmanyBits(center & state.getBlackPieces() & state.getPieces(Chess::pawns));

        //skoczki 
        score += 50 * howmanyBits(center & state.getWhitePieces() & state.getPieces(Chess::knights));
        score += 30 * howmanyBits(aroundcenter & state.getWhitePieces() & state.getPieces(Chess::knights));

        score -= 50 * howmanyBits(center & state.getBlackPieces() & state.getPieces(Chess::knights));
        score -= 30 * howmanyBits(aroundcenter & state.getBlackPieces() & state.getPieces(Chess::knights));


        // Mobility judgment
        state.generate_mobilty_table();
        //wieze 
        score += mobiltywage * state.mobiltyTable[Chess::white][Chess::rooks];
        score -= mobiltywage * state.mobiltyTable[Chess::black][Chess::rooks];
                 
        
        score += mobiltywage * state.mobiltyTable[Chess::white][Chess::bishops];
        score -= mobiltywage * state.mobiltyTable[Chess::black][Chess::bishops];
                 
        //queens 
        score += 2*mobiltywage * state.mobiltyTable[Chess::white][Chess::queens];
        score -= 2*mobiltywage * state.mobiltyTable[Chess::black][Chess::queens];


        ///// Position judgment 






        return score;

    }

    int engine::search(int depth, int alfa, int beta)
    {
        if (depth == 0)
            return evalulate();
        state.generateLegalMoves(this->movelist[depth]);
        if (state.isWhiteMove())
        {
            int max = -1000000;
            for (int i = 0; i < movelist[depth].count; i++)
            {
                Chess::Move move = this->movelist[depth].moves[i];
                state.Simulate_Move(move);
                int actual = search(depth - 1, alfa, beta);
                state.undo_Move();
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
            int min = 1000000;
            for (int i = 0; i < movelist[depth].count; i++)
            {
                Chess:: Move move = this->movelist[depth].moves[i];
                state.Simulate_Move(move);
                int actual = search(depth - 1, alfa, beta);
                state.undo_Move();
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

    Chess::Move engine::bestMove(int depth, int alfa, int beta)
    {
        
       state.generateLegalMoves(this->movelist[depth]);
        
        /// rojechana pamiec 

        Chess::Move bestMove = 0;// state pokazuje w roznych polach obiektu unable to read memory
        if (state.isWhiteMove()) // tu b³¹d 
        {
            int max = -10000000;
            for (int i = 0; i < movelist[depth].count; i++)
            {
                Chess::Move move = this->movelist[depth].moves[i];
                state.Simulate_Move(move);
                int actual = search(depth - 1, alfa, beta);
                state.undo_Move();
                if (actual > max)
                {
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
            int min = 10000000;
            for (int i = 0; i < movelist[depth].count; i++)
            {
                Chess::Move move = this->movelist[depth].moves[i];
                state.Simulate_Move(move);
                int actual = search(depth - 1, alfa, beta);
                state.undo_Move();
                if (actual < min)
                {
                    min = actual;
                    bestMove = move;
                }
                if (actual < beta)
                    alfa = actual;
                if (alfa >= beta)
                    break;
            }
        }
        return bestMove;

    }
}