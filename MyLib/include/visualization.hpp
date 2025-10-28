#include"SFML/Graphics.hpp"
#include"chesstate.hpp"
#include"piece.hpp"
namespace Vis {
	class visualization {
	private:
		std::unique_ptr<Piece>Pieces[64] = {};
		std::unique_ptr<sf::Sprite> chessboard = {};
		std::vector<sf::CircleShape> PosMoves;
		sf::Texture whiteTex[Chess::sizePieces];
		sf::Texture blackTex[Chess::sizePieces];
		sf::Texture chesstex;
		int upoffset = 0;
		int rightoffset= 0;
		int downoffset= 0;
		int leftoffset= 0;
	public:
		visualization();
		void loadTextures();
		void setBegPos(Chess::ChessState& state);
		void pom(U64 a, sf::Texture& tex);
		void draw(sf::RenderWindow& window);
		void simulateMove(int from, int to);
		void createPosMoves(int(&currentPossibleMoves)[50], int count);
		void deletePotentialMoves();

		static sf::Vector2f stateCorToWindowCor(int a);
		static int windowCorToStateCor(sf::Vector2f vec);
		

	};
}