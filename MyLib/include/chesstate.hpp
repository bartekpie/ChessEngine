#pragma once
#include"types.hpp"
#include"bitboard.hpp"
#include "const.hpp"
#include <stack>
namespace Chess {


	enum Pieces {
		none = -1, pawns = 0, knights, bishops, rooks, queens, kings, sizePieces
	};
	enum Colors {
		black = 0, white = 1, sizeColors
	};
	enum state {
		game = 0, Pat , Mate
	};
	struct Undo
	{
		int from{-1};
		int to{-1};
		Pieces moved{none};
		Pieces captured{none};
		bool white_move{ 1 };
	};
	struct MoveList
	{
	public:
		Move moves[256] = {};
		int count{0};
	};
	class ChessState {
	public:
		
		ChessState();
		int mobiltyTable[2][sizePieces] = { 0 };
		int generateLegalMoves(MoveList& moves);
		int generateLegalMoves();
		void generate_mobilty_table();
		void Simulate_Move(Move move);
		void undo_Move();
		void generatePseudoLegalMoves();
		void generatePseudoLegalMoves(MoveList& move);
		MoveList getMoves() const { return moves; }
		bool isWhiteMove() const { return white_move; }
		U64 getPieces(int piece) const { return pieces[piece]; }
		U64 getWhitePieces() const { return whitePieces; }
		U64 getBlackPieces() const { return blackPieces; }
		U64 getEmptySpaces() const { return emptySpaces; }
		U64 getCurrentMovePieces()const { if (white_move)return whitePieces; return blackPieces; }
		static Move code_move(int from, int to);
		Undo historyTop() { return history.top(); }
	private:
		
		U64 blackPieces;  /// opis szachownicy
		U64 whitePieces;
		U64 emptySpaces;
		U64 pieces[sizePieces];
		bool white_move;
		
		/// struktura do szybkiego zapisywania ruchów
		MoveList moves;
		MoveList checkhelper;

		/// tablice do szybkiego generowania ruchów
		U64 NE[64];
		U64 SE[64];
		U64 NW[64];
		U64 SW[64];
		U64 North[64];
		U64 West[64];
		U64 South[64];
		U64 East[64];
		U64 Knights[64];
		U64 King[64];
		std::stack<Undo> history;

		

		U64 MyPieces();

		U64 OpponentPieces();

		void encode(U64 a, int from, MoveList& move);



		void calculate_Pawn_Moves(MoveList& move);
		U64 calculate_bishop_moves(int square);
		U64 calculate_rook_moves(int square);
		U64 calculate_queen_moves(int sq);
		U64 calculate_Knight_moves(int a);
		U64 calculate_king_moves(int a);

		int filter_king_captures(MoveList& move, MoveList& helper);

		///statyczne 
		static bool movePiece(U64& bitboard, int from, int to);
		static U64 row(int a);
		static U64 column(int a);
		static U64 knightMoves(int square);
		static void generate_knight_table(U64(&Knights)[64]);
		static void generate_rook_tables(U64(&N_)[64], U64(&S_)[64], U64(&E_)[64], U64(&W_)[64]);
		static void generate_bishop_tables(U64(&NE_)[64], U64(&NW_)[64], U64(&SE_)[64], U64(&SW_)[64]);
		static void generate_king_table(U64(&Kings)[64]);
		friend class ChessStateTest;
	};
	

}