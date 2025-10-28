#include "chesstate.hpp"
#include "piece.hpp"
#include"SFML/Graphics.hpp"
#include "console.hpp"
#include"engine.hpp"
#include"visualization.hpp"
#include "types.hpp"
#include<fstream>
namespace Game {
	enum class mode {
		multiplayer = 0,
		againstengine = 1
	};
	struct SaveMove {
		std::ofstream plik{"resources/zapis_parti.txt"};
		int movescount{ 0 };
		std::array<std::string,6> names ={
			"P", "K", "B", "R", "Q", "K"
		};
		std::string fromIndex(int index);
		std::string createString(Chess::Undo);
	};
	class Game {
	public:
		Game();
		
	
		void mainloop();
	private:
		mode mode = mode::againstengine;
		Chess::ChessState state;
		sf::RenderWindow window;
		Vis::visualization visualisation;
		Engine::engine engine{ state };
		SaveMove saver;
		int movecount{ 0 };
		int currentPossibleMoves[50] = {};
		int currentCount = 0;
		int clickedFrom = -1;
		void processClick(int position);
		void uploadPosMoves(int clickePos);
		void movePiece(int clickedPos);

		
		
		void addCurrPotMovesfromChessState(int clickedPos);
		void clearPotMoves();
		void SimulateMove(int clickedPos);
		void SimulateMove(Move move);
		void generateMovesAndcheckMate();
		
	};
}
