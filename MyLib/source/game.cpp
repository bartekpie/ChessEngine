#include "game.hpp"

namespace Game {
    
    Game::Game() : window(sf::VideoMode({ 1600,1600 }), "Chess")
    {  
           visualisation.setBegPos(state);
           state.generateLegalMoves();
    }  
    bool Game::recreateGame()
    {
        bool isSavedLegal = false;
        Chess::MoveList doneMoves;
        Chess::Move savedMove;
        std::queue<Chess::Move> queue = saver.getQueue();
        while(queue.empty()==false)
        {
            isSavedLegal = false;
            savedMove = queue.front();
            queue.pop();
            state.generateLegalMoves();
            Chess::MoveList movelist= state.getMoves();
            for(int i = 0;i<movelist.count;i++)
                if (savedMove == movelist.moves[i])
                {
                    isSavedLegal = true;
                    break;
                }
            
            if (isSavedLegal == false)
            {
                break;
            }
            else
            {
                state.Simulate_Move(savedMove);
                doneMoves.moves[doneMoves.count++] = savedMove;
            }
        }
        if (isSavedLegal == false)
        {
            for (int i = 1; i <= doneMoves.count; i++)
            {
                state.undo_Move();
            }
            return 0;
        }
        for (int i = 0; i < doneMoves.count; i++)
        {
            int from = (doneMoves.moves[i] >> 6) & 0x3f;
            int to = (doneMoves.moves[i] & 0x3f);
            visualisation.simulateMove(from,to);
        }
        return 1;
    }
    void Game::processClick(int position)
    {
        if (mode == mode::multiplayer)
        {
            this->uploadPosMoves(position);
            visualisation.createPosMoves(currentPossibleMoves, currentCount);
            this->movePiece(position);
        }
        else {

            if (state.isWhiteMove())
            {
                this->uploadPosMoves(position);
                visualisation.createPosMoves(currentPossibleMoves, currentCount);
                this->movePiece(position);
            }
    
        }
   
    }
    void Game::Game::uploadPosMoves(int clickedPos)
    {

        if (state.isWhiteMove())
        {
            if (getBit(state.getWhitePieces(), clickedPos))
            {
                addCurrPotMovesfromChessState(clickedPos);
            }

        }
        if (!state.isWhiteMove())
        {
            if (getBit(state.getBlackPieces(), clickedPos))
            {
                addCurrPotMovesfromChessState(clickedPos);
            }
        }
    }
    void Game::Game::movePiece(int clickedPos)
    {
        if (clickedFrom < 0) return;
        if (getBit(~(state.getCurrentMovePieces()), clickedPos))
        {
            for (int i = 0; i < currentCount; i++)
            {
                if (clickedPos == currentPossibleMoves[i])
                {
                    SimulateMove(clickedPos);
                    generateMovesAndcheckMate();
                    break;
                }
            }
        }
    }
    void Game::addCurrPotMovesfromChessState(int clickedPos)
    {
        clearPotMoves();
        for (int i = 0; i < state.getMoves().count; i++)
        {
            int from = (state.getMoves().moves[i] >> 6) & 0x3F;
            if (from == clickedPos)
            {
                currentPossibleMoves[currentCount++] = state.getMoves().moves[i] & 0x3F;
                clickedFrom = from;
            }
        }
    }
    void Game::clearPotMoves()
    {
        
        visualisation.deletePotentialMoves(); 
        clickedFrom = -1;
        currentCount = 0;
    }
    void Game::SimulateMove(int clickedPos)
    {
        state.Simulate_Move(Chess::ChessState::code_move(clickedFrom, clickedPos));
        std::cout<<saver.saveInFile(saver.createString(state.historyTop()));
        visualisation.simulateMove(clickedFrom, clickedPos);
        clearPotMoves();
        
    }
    void Game::SimulateMove(Chess::Move move)
    {
        state.Simulate_Move(move);
        std::cout<<saver.saveInFile(saver.createString(state.historyTop()));
        visualisation.simulateMove( (move>>6)&0x3f, move & 0x3f);
        clearPotMoves();

    }
    
    void Game::generateMovesAndcheckMate()
    {
        switch (state.generateLegalMoves()) {
        case(Chess::game):
            break;
        case(Chess::Pat):
            std::cout << "Pat";
            window.close();
            break;
        case(Chess::Mate):
            std::cout << "Mat";
            window.close();
            break;
        }
    }
    void Game::Game::mainloop()
    {
        
        
        while (window.isOpen())
        {
            while (const std::optional event = window.pollEvent())
            {
                if (event->is<sf::Event::Closed>())
                    window.close();
                if (event->is<sf::Event::MouseButtonPressed>() && event->getIf<sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Left)
                {
                    sf::Vector2f windowPos  = sf::Vector2f(sf::Mouse::getPosition(window));
                    int statePosition = Vis::visualization::windowCorToStateCor(windowPos);
                    if (statePosition < 64 && statePosition >= 0)
                    {
                        processClick(statePosition);
                    }
                 
                }
                
            }
            if (mode == mode::againstengine) {
                if (state.isWhiteMove() == 0) {
                    Chess::Move a = engine.bestMove(2, -1000000, 100000);
                    this->SimulateMove(a);
                    this->generateMovesAndcheckMate();
                }
            }
            visualisation.draw(window);
        }
    }
   
}





