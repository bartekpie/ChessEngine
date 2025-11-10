#include "chesstate.hpp"
#include "piece.hpp"
#include "bitboard.hpp"
#include"SFML/Graphics.hpp"
#include "console.hpp"
#include"engine.hpp"
#include"visualization.hpp"
#include "saver.hpp"
#include<fstream>
#include <queue>
namespace Game {
	enum class mode {
		multiplayer = 0,
		againstengine = 1
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
		Saver saver;
		int movecount{0};
		int currentPossibleMoves[50] = {};
		int currentCount = 0;
		int clickedFrom = -1;

		bool recreateGame();
		bool movesFromFileLegal(Chess::MoveList& doneMoves);

		void processClick(int position);
		void uploadPosMoves(int clickePos);
		void movePiece(int clickedPos);
		void addCurrPotMovesfromChessState(int clickedPos);
		void clearPotMoves();
		void SimulateMove(int clickedPos);
		void SimulateMove(Chess::Move move);
		void generateMovesAndcheckMate();
		
	};
}
