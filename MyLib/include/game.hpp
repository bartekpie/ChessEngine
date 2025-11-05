#include "chesstate.hpp"
#include "piece.hpp"
#include "bitboard.hpp"
#include"SFML/Graphics.hpp"
#include "console.hpp"
#include"engine.hpp"
#include"visualization.hpp"
#include "types.hpp"
#include<fstream>
#include <queue>
namespace Game {
	enum class mode {
		multiplayer = 0,
		againstengine = 1
	};
	struct SaveMove {
		int movescount{0};
		std::queue<Move> todomoves;
		std::array<char,6> names ={
			'P', 'K', 'B', 'R', 'Q', 'K'
		};
		std::string fromIndex(int index);
		int strToint(std::string& s);
		std::string createString(Chess::Undo undo);
		int saveInFile(std::string text);
		void fromFiletoQueue();
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
		int movecount{0};
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
