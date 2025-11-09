#include "console.hpp"

int square_from_string(std::string str) {
    if (str.length() != 2) return -1;
    int file = str[0] - 'a';
    int rank = str[1] - '1';
    if (file < 0 || file > 7 || rank < 0 || rank > 7) return -1;
    return rank * 8 + file;
}



Chess::Move parse_move(std::string input) {
    if (input.length() != 4) return 0;
    int from = square_from_string(input.substr(0, 2));
    int to = square_from_string(input.substr(2, 2));
    if (from == -1 || to == -1) return 0;
    return Chess::ChessState::code_move(from, to);
}

void handle_move_input(std::string input, Chess::ChessState& state) {


    Chess::Move userMove = parse_move(input);

    bool found = false;

    for (int i = 0; i < state.getMoves().count; ++i) {
        if (state.getMoves().moves[i] == userMove) {
            state.Simulate_Move(state.getMoves().moves[i]);
            found = true;
            std::cout << state.getMoves().moves[i] << std::endl;;
            break;
        }
    }

    if (!found) {
        std::cout << "Nielegalny ruch!" << std::endl;
    }
}
void setLetter(U64 wzor, char symbol, char board[64])
{
    while (wzor != 0)
    {
        int a = std::countr_zero(wzor);
        board[a] = symbol;
        resetBit(wzor, Square(a));
    }
}
void print_board(Chess::ChessState state, char board[64]) {
    char EMPTY = '.';
    std::fill(board, board + 64, EMPTY);

    U64 whitePawns = state.getWhitePieces() & state.getPieces(Chess::pawns);
    setLetter(whitePawns, 'p', board);

    U64 blackPawns = state.getBlackPieces() & state.getPieces(Chess::pawns);
    setLetter(blackPawns, 'P', board);

    U64 whiteKnights = state.getWhitePieces() & state.getPieces(Chess::knights);
    setLetter(whiteKnights, 'n', board);

    U64 blackKnights = state.getBlackPieces() & state.getPieces(Chess::knights);
    setLetter(blackKnights, 'N', board);

    U64 whiteRooks = state.getWhitePieces() & state.getPieces(Chess::rooks);
    setLetter(whiteRooks, 'r', board);

    U64 blackRooks = state.getBlackPieces() & state.getPieces(Chess::rooks);
    setLetter(blackRooks, 'R', board);

    U64 whiteBishops = state.getWhitePieces() & state.getPieces(Chess::bishops);
    setLetter(whiteBishops, 'b', board);

    U64 blackBishops = state.getBlackPieces() & state.getPieces(Chess::bishops);
    setLetter(blackBishops, 'B', board);

    U64 whiteQueens = state.getWhitePieces() & state.getPieces(Chess::queens);
    setLetter(whiteQueens, 'q', board);

    U64 blackQueens = state.getBlackPieces() & state.getPieces(Chess::queens);
    setLetter(blackQueens, 'Q', board);

    U64 whiteKing = state.getWhitePieces() & state.getPieces(Chess::kings);
    setLetter(whiteKing, 'k', board);

    U64 blackKing = state.getBlackPieces() & state.getPieces(Chess::kings);
    setLetter(blackKing, 'K', board);





    for (int rank = 7; rank >= 0; --rank) {
        std::cout << rank + 1 << " | ";
        for (int file = 0; file < 8; ++file) {
            Square sq = (Square)(rank * 8 + file);
            std::cout << board[sq] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "   -----------------" << std::endl;
    std::cout << "    A B C D E F G H" << std::endl << std::endl;
}