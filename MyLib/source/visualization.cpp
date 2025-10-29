#include "visualization.hpp"

Vis::visualization::visualization()
{
    loadTextures();
}

void Vis::visualization::loadTextures()
{
    whiteTex[Chess::pawns].loadFromFile(("resources/white_pawn.png"));
    whiteTex[Chess::knights].loadFromFile(("resources/white_knight.png"));
    whiteTex[Chess::bishops].loadFromFile(("resources/white_bishop.png"));
    whiteTex[Chess::rooks].loadFromFile(("resources/white_rock.png"));
    whiteTex[Chess::queens].loadFromFile(("resources/white_queen.png"));
    whiteTex[Chess::kings].loadFromFile(("resources/white_king.png"));

    blackTex[Chess::pawns].loadFromFile(("resources/black_pawn.png"));
    blackTex[Chess::knights].loadFromFile(("resources/black_knight.png"));
    blackTex[Chess::bishops].loadFromFile(("resources/black_bishop.png"));
    blackTex[Chess::rooks].loadFromFile(("resources/black_rock.png"));
    blackTex[Chess::queens].loadFromFile(("resources/black_queen.png"));
    blackTex[Chess::kings].loadFromFile(("resources/black_king.png"));

    chesstex.loadFromFile("resources/chessboard.png");
    chessboard = std::make_unique<sf::Sprite>(chesstex);
}

void Vis::visualization::setBegPos(Chess::ChessState& state)
{
    for (int i = 0; i < Chess::sizePieces; i++)
    {
        U64 type = state.getWhitePieces() & state.getPieces(Chess::Pieces(i));
        pom(type, whiteTex[i]);
    }
    for (int i = 0; i < Chess::sizePieces; i++)
    {
        U64 type = state.getBlackPieces() & state.getPieces(Chess::Pieces(i));
        pom(type, blackTex[i]);
    }
}

void Vis::visualization::draw(sf::RenderWindow& window)
{
    window.clear();
    window.draw(*chessboard);
    for (int i = 0; i < 64; i++)
    {
        if (Pieces[i])
            window.draw(*Pieces[i]);
    }
    for (sf::CircleShape& a : PosMoves)
    {
        window.draw(a);
    }
    window.display();
}

void Vis::visualization::simulateMove(int from, int to)
{
    Pieces[to] = std::move(Pieces[from]);
    if(Pieces[to])
    Pieces[to]->setPosition(stateCorToWindowCor(to));
    else {
        std::cout << "b³ad w viusal::simualtemove";
    }
}

void Vis::visualization::createPosMoves(int (&currentPossibleMoves) [50], int currentCount)
{
    for (int i = 0; i < currentCount; i++)
    {
        sf::Vector2f position = stateCorToWindowCor(currentPossibleMoves[i]);
        sf::CircleShape posMove;
        posMove.setRadius(30.f);
        posMove.setOrigin(sf::Vector2f(35.f, 35.f));
        posMove.setPosition(position);
        posMove.setFillColor(sf::Color(255, 0, 0, 150));
        posMove.setOutlineThickness(2.0f);
        posMove.setOutlineColor(sf::Color::Red);
        PosMoves.push_back(posMove);
    }
}

void Vis::visualization::deletePotentialMoves()
{
    PosMoves.clear();
}

void Vis::visualization::pom(U64 a, sf::Texture& tex)
{
    while (a != 0)
    {
        int actual = std::countr_zero(a);
        sf::Vector2f pos = stateCorToWindowCor(actual);
        Pieces[actual] = std::make_unique<Piece>(tex, pos);
        resetBit(a, actual);
    }


}
sf::Vector2f  Vis::visualization::stateCorToWindowCor(int a)
{
    int row = a % 8;
    int column = 7 - (a / 8);
    float x = row * 200.f  + 100.f;
    float y = column * 200.f  + 100.f;
    return{ x,y };

}
int  Vis::visualization::windowCorToStateCor(sf::Vector2f vec)
{
    int column = static_cast<int>(((vec.y ) / 200));
    int row = static_cast<int>((vec.x ) / 200);
    return (7 - column) * 8 + row;
}