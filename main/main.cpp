
#include "game.hpp"
#include <iostream>

int main() {
    Game::Game game;
    game.mainloop(); 
   
    
   
    /*Chess::ChessState real;   
    Engine::engine engine(real);
    std::string input;
    char board[64];
    while (true) {
        print_board(real, board);
        if (real.isWhiteMove())
        {
            std::cout << (real.isWhiteMove() ? "Bialy" : "Czarny") << " ruch :  ";
            std::cin >> input;

            if (input == "end")
                break;
            if (input == "undo")
            {
                real.undo_Move();
                continue;
            }
            real.generateLegalMoves();
            handle_move_input(input, real);


        }
        else {

            real.Simulate_Move(engine.bestMove(max_depth, -1000000, 10000000));
        }
    }

    std::cout << "Koniec gry." << std::endl;
    return 0;*/
}

