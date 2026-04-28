
#include "engine.hpp"
#include <iostream>

int main() {

    const std::string mate_in_one = "r1bqkbnr/pp1ppppp/2n5/2p5/2B1P3/5Q2/PPPP1PPP/RNB1K1NR w KQkq - 4 4";
    const std::string middle_game = "r1bq1rk1/pp3ppp/2n1pn2/2bp4/2P1P3/2N2N2/PP3PPP/R1BQ1RK1 w - - 0 9";

    Engine::engine eng;

    int depth = 10;
    int alpha = -100000;
    int beta = 100000;
    std::thread searchThread(&Engine::engine::iterativeDeepening, &eng, depth, alpha, beta);
    std::thread statisticsThread(&Engine::engine::printStatistics, &eng);

    searchThread.join();
    statisticsThread.join();

    return 0;

   
    
    
    
    
    
    
    
    
    
    
    
    
    

}

