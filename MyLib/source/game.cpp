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
    int SaveMove::strToint(std::string& s)
    {
        if (s.size() != 2)
            throw std::invalid_argument("Niepoprawny format pola: " + s);

        char file = s[0]; 
        char rank = s[1]; 

        if (file < 'a' || file > 'h' || rank < '1' || rank > '8')
            throw std::out_of_range("Niepoprawne wspó³rzêdne pola: " + s);

        int fileIndex = file - 'a';
        int rankIndex = rank - '1';

        int squareIndex = rankIndex * 8 + fileIndex;
        return squareIndex;
    }
    inline std::string SaveMove::createString(Chess::Undo undo)
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
        int a{ 0 };
        int offset{ 0 };
        std::string move = "";
        while (std::getline(plik, move))
        {
            a++;
            if (a > 9)
                offset = 1;
            if (a > 99)
                offset = 2;
            if (move == "")
                break;
            std::string from;
            std::string to;
            from += move[4 + offset];
            from += move[5 + offset];
            to += move[7 + offset];
            to += move[8 + offset];
            for (int i = 0; i < 6; i++)
            {
                if (to[0] == names[i]) {
                    to = "";
                    to += move[8+offset];
                    to += move[9+offset];
                    break;
                }
            }
            
            int from_int = strToint(from);
            int to_int = strToint(to);
            todomoves.push(Chess::ChessState::code_move(from_int, to_int));
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





