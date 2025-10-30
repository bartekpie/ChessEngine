#include "game.hpp"

namespace Game {
    std::string SaveMove::fromIndex(int index)
    {
        int file = index % 8; 
        int rank = index / 8;     
        char fileChar = 'a' + file;
        char rankChar = '1' + rank;
        return std::string{ fileChar, rankChar };
    }
    inline std::string SaveMove::createString(Chess::Undo)
    {
        movescount++;
        std::string from = fromIndex(undo.from);
        std::string to = fromIndex(undo.to);
        std::string moved = "";
        std::string captured = "";
        if (undo.moved != -1)
            moved = names[undo.moved];
        if (undo.captured != -1)
            captured = names[undo.captured];
        std::string count = std::to_string(this->movescount);
        return (count + ". " + moved + from + " " + captured + to);
    }
    int SaveMove::saveInFile(std::string text)
    {
        std::ofstream plik{ "resources/zapis_parti.txt" };
        if (!plik.is_open()) 
            return 0; 
        plik << text << std::endl;
        return 1;
    }
    void SaveMove::fromFiletoQueue()
    {
        std::ifstream plik{ "resources/zapis_parti.txt" };
        int a{ 1};
        int offset{ 0 };
        string move = "";
        while (std::getline(plik, move))
        {
            if (move = "")
                break;
            std::string from = move[5 + offset] + move[6 + offset];
            std::string to = move[8 + offset] + move[9 + offset];
            for (int i = 0; i < 6; i++)
            {
                if (to[1] == names[i]) {
                    to = move[9+offset] + move[10+offset];
                    break;
                }
            }
            a++;
            if (a > 9)
                offset = 1;
            if (a > 99)
                offset =2 ;
            int from_int = static_cast<int>(from);
            int to_int = static_cast<int>(to);
            todomoves.push(code_move(from_int, to_int));
        }
    }
    Game::Game() : window(sf::VideoMode({ 1600,1600 }), "Chess")
    {  
           visualisation.setBegPos(state);
           state.generateLegalMoves();
    }  
    void Game::processClick(int position)
    {
        saver.fromFiletoQueue();
        std::cout << saver.todomoves.pop();
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
        saver.saveInFile(saver.createString(state.historyTop()));
        visualisation.simulateMove(clickedFrom, clickedPos);
        clearPotMoves();
        
    }
    void Game::SimulateMove(Move move)
    {
        state.Simulate_Move(move);
        std::cout<<(saver.saveInFile(saver.createString(state.historyTop())));
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
                    Move a = engine.bestMove(2, -1000000, 100000);
                    this->SimulateMove(a);
                    this->generateMovesAndcheckMate();
                }
            }
            visualisation.draw(window);
        }
    }
   
}





