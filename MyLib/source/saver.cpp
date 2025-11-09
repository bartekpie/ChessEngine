
#include"saver.hpp"


std::string Saver::fromIndex(int index)
{
    int file = index % 8;
    int rank = index / 8;
    char fileChar = 'a' + file;
    char rankChar = '1' + rank;
    return std::string{ fileChar, rankChar };
}
int Saver::strToint(std::string& s)
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
std::string Saver::createString(Chess::Undo undo)
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
bool Saver::saveInFile(std::string text)
{
    std::ofstream plik{ "resources/zapis_parti.txt", std::ios::app };
    if (!plik.is_open())
        return 0;
    plik << text;
    plik << std::endl;
    return 1;
}
void Saver::fromFiletoQueue()
{
    for (int i = 0; i < todomoves.size(); i++)
    {
        todomoves.pop();
    }
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
                to += move[8 + offset];
                to += move[9 + offset];
                break;
            }
        }

        int from_int = strToint(from);
        int to_int = strToint(to);
        todomoves.push(Chess::ChessState::code_move(from_int, to_int));
    }
}
std::queue<Chess::Move> Saver::getQueue()
{
    return todomoves;
}