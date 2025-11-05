#include "chesstate.hpp"
namespace Chess {


	U64 ChessState::MyPieces()
	{
		if (white_move)
			return whitePieces;
		else {
			return blackPieces;
		}
	}

	U64 ChessState::OpponentPieces()
	{
		if (white_move == 0)
			return whitePieces;
		else {
			return blackPieces;
		}
	}

	bool ChessState::movePiece(U64& bitboard, int from, int to) {
		if (getBit(bitboard, from)) {
			resetBit(bitboard, from);
			setBit(bitboard, to);
			return true;
		}
		return false;
	}



	ChessState::ChessState()
	{
		generate_knight_table(Knights);
		generate_bishop_tables(NE, NW, SE, SW);
		generate_rook_tables(North, South, West, East);
		generate_king_table(King);
		pieces[pawns] = (row(6) | row(1));
		pieces[rooks] = (row(7) | row(0)) & (column(0) | column(7));
		pieces[knights] = (row(7) | row(0)) & (column(1) | column(6));
		pieces[bishops] = (row(7) | row(0)) & (column(2) | column(5));
		pieces[queens] = (row(7) | row(0)) & column(3);
		pieces[kings] = (row(7) | row(0)) & column(4);
		blackPieces = row(7) | row(6);
		whitePieces = row(1) | row(0);
		emptySpaces = ~(whitePieces | blackPieces);
		white_move = 1;
	}

	void ChessState::generatePseudoLegalMoves()
	{
		moves.count = 0;
		U64 myPieces = MyPieces();
		U64 opponentPieces = OpponentPieces();
		U64 MyKnights = myPieces & pieces[knights];
		while (MyKnights != 0)
		{
			int from = std::countr_zero(MyKnights);
			resetBit(MyKnights, Square(from));
			U64 to = calculate_Knight_moves(from);
			mobiltyTable[white_move][knights] = howmanyBits(to);
			encode(to, from, moves);
		}
		U64 MyBishops = myPieces & pieces[bishops];
		while (MyBishops != 0)
		{
			int from = std::countr_zero(MyBishops);
			resetBit(MyBishops, Square(from));
			U64 to = calculate_bishop_moves(from);
			mobiltyTable[white_move][bishops] = howmanyBits(to);
			encode(to, from, moves);
		}

		U64 MyRooks = myPieces & pieces[rooks];
		while (MyRooks != 0)
		{
			int from = std::countr_zero(MyRooks);
			resetBit(MyRooks, Square(from));
			U64 to = calculate_rook_moves(from);
			mobiltyTable[white_move][rooks] = howmanyBits(to);
			encode(to, from, moves);
		}
		U64 MyQueens = myPieces & pieces[queens];
		while (MyQueens != 0)
		{
			int from = std::countr_zero(MyQueens);
			resetBit(MyQueens, Square(from));
			U64 to = calculate_queen_moves(from);
			mobiltyTable[white_move][queens] = howmanyBits(to);
			encode(to, from, moves);
		}
		U64 MyKings = myPieces & pieces[kings];
		while (MyKings != 0)
		{
			int from = std::countr_zero(MyKings);
			resetBit(MyKings, Square(from));
			U64 to = calculate_king_moves(from);
			mobiltyTable[white_move][kings] = howmanyBits(to);
			encode(to, from, moves);
		}
		calculate_Pawn_Moves(moves);

	}

	void ChessState::generatePseudoLegalMoves(MoveList& move)
	{
		move.count = 0;
		U64 myPieces = MyPieces();
		U64 opponentPieces = OpponentPieces();
		U64 MyKnights = myPieces & pieces[knights];
		while (MyKnights != 0)
		{
			int from = std::countr_zero(MyKnights);
			resetBit(MyKnights, Square(from));
			U64 to = calculate_Knight_moves(from);
			mobiltyTable[white_move][knights] = howmanyBits(to);
			encode(to, from, move);
		}
		U64 MyBishops = myPieces & pieces[bishops];
		while (MyBishops != 0)
		{
			int from = std::countr_zero(MyBishops);
			resetBit(MyBishops, Square(from));
			U64 to = calculate_bishop_moves(from);
			mobiltyTable[white_move][bishops] = howmanyBits(to);
			encode(to, from, move);
		}

		U64 MyRooks = myPieces & pieces[rooks];
		while (MyRooks != 0)
		{
			int from = std::countr_zero(MyRooks);
			resetBit(MyRooks, Square(from));
			U64 to = calculate_rook_moves(Square(from));
			mobiltyTable[white_move][rooks] = howmanyBits(to);
			encode(to, from, move);
		}
		U64 MyQueens = myPieces & pieces[queens];
		while (MyQueens != 0)
		{
			int from = std::countr_zero(MyQueens);
			resetBit(MyQueens, Square(from));
			U64 to = calculate_queen_moves(Square(from));
			mobiltyTable[white_move][queens] = howmanyBits(to);
			encode(to, from, move);
		}
		U64 MyKings = myPieces & pieces[kings];
		while (MyKings != 0)
		{
			int from = std::countr_zero(MyKings);
			resetBit(MyKings, Square(from));
			U64 to = calculate_king_moves(from);
			mobiltyTable[white_move][kings] = howmanyBits(to);
			encode(to, from, move);
		}
		
		calculate_Pawn_Moves(move);

	}

	int ChessState::generateLegalMoves(MoveList& move)
	{
		move.count = 0;
		checkhelper.count = 0;
		generatePseudoLegalMoves(move);
		int a = filter_king_captures(move, checkhelper);
		
		return(a);
	}

	int ChessState::generateLegalMoves()
	{
		moves.count = 0;
		checkhelper.count = 0;
		generatePseudoLegalMoves(this->moves);
		return(filter_king_captures(moves, checkhelper));
	}

	void ChessState::generate_mobilty_table()
	{

		white_move = !white_move;

		mobiltyTable[white_move][knights] = 0;
		mobiltyTable[white_move][bishops] = 0;
		mobiltyTable[white_move][rooks] = 0;
		mobiltyTable[white_move][queens] = 0;

		U64 myPieces = MyPieces();
		U64 MyKnights = myPieces & pieces[knights];
		while (MyKnights != 0)
		{
			int from = std::countr_zero(MyKnights);
			resetBit(MyKnights, from);
			mobiltyTable[white_move][knights] = howmanyBits(calculate_Knight_moves(from));
		}
		U64 MyBishops = myPieces & pieces[bishops];
		while (MyBishops != 0)
		{
			int from = std::countr_zero(MyBishops);
			resetBit(MyBishops, Square(from));
			mobiltyTable[white_move][bishops] = howmanyBits(calculate_bishop_moves(from));
		}

		U64 MyRooks = myPieces & pieces[knights];
		while (MyRooks != 0)
		{
			int from = std::countr_zero(MyRooks);
			resetBit(MyRooks, Square(from));
			mobiltyTable[white_move][rooks] = howmanyBits(calculate_rook_moves(from));
		}
		U64 MyQueens = myPieces & pieces[queens];
		while (MyQueens != 0)
		{
			int from = std::countr_zero(MyQueens);
			resetBit(MyQueens, Square(from));
			mobiltyTable[white_move][queens] = howmanyBits(calculate_queen_moves(from));
		}
		white_move = !white_move;

	}

	void ChessState::Simulate_Move(Move move)
	{
		Undo undo;
		undo.from = (move >> 6) & 0x3F;
		undo.to = move & 0x3F;
		int from = undo.from;
		int to = undo.to;


		/// dajemy przestzen do ruchu czyli usuwamy wszelkie bity mog¹ce byæ ustawione
		/// na polu to. Jeœli znajdziemy bit ustawiony spisujemy jego wartoœc do undo.captured
		resetBit(blackPieces, to);
		resetBit(whitePieces, to);
		for (int i = 0; i < sizePieces; i++)
		{
			if (getBit((pieces[i]), to))
			{
				resetBit(pieces[i], to);
				undo.captured = static_cast<Pieces>(i);
				break;
			}
		}
		
		movePiece(whitePieces, from, to);
		movePiece(blackPieces, from, to);
		bool movedsth = 0;
		for (int i = 0; i < sizePieces; i++)
		{
			if (movePiece(pieces[i], from, to))
			{
				undo.moved = static_cast<Pieces>(i);
				movedsth = true;
				break;
			}
		}
		if (!movedsth) {
			std::cout << "B£¥D: Simulate_Move nie znalaz³ figury do przesuniêcia z pola "
				<< from << " na " << to << "\n";
		}
		/// Po ruchu aktualizujemy pozycje pustych pól i zmieniamy gracza
		emptySpaces = ~(whitePieces | blackPieces);
		undo.white_move = white_move;
		history.push(undo);
		white_move = !white_move;

	}
	void ChessState::undo_Move()
	{

		if (history.empty())
		{
			std::cout << "history jest puste Metoda undo_Move" << std::endl;
			return;
		}


		Undo actual = history.top();
		if (actual.moved < 0 || actual.moved >= sizePieces) {
			std::cout << "B£¥D: undo_Move dosta³ nieprawid³owy typ figury: " << actual.moved << "\n";
			history.pop();
			return;
		}
		history.pop();
		if (actual.from == actual.to && actual.from == 0)
			return;

		///Przywracamy figure na pole from i usuwamy z pola to

		resetBit(pieces[actual.moved], actual.to);
		setBit(pieces[actual.moved], actual.from);
		///Jeœli by³o bicie to przywracamy zbity pionek
		if (actual.captured != -1)
		{
			setBit(pieces[actual.captured], actual.to);
			if (actual.white_move)
				setBit(blackPieces, actual.to);
			else
				setBit(whitePieces, actual.to);
		}
		///Przywracamy dobre bitboardy white i black Pieces
		if (actual.white_move)
		{
			setBit(whitePieces, actual.from);
			resetBit(whitePieces, actual.to);
		}
		else
		{
			setBit(blackPieces, actual.from);
			resetBit(blackPieces, actual.to);
		}
		/// Przywracamy reszcie zmiennym w³asciwe dane
		emptySpaces = ~(whitePieces | blackPieces);

		white_move = actual.white_move;


	}
	Move ChessState::code_move(int from, int to)
	{
		return ((from & 0x3f) << 6) | (to & 0x3f);
	}

	void ChessState::encode(U64 a, int from, MoveList& move)
	{
		while (a != 0)
		{
			int to = std::countr_zero(a);
			resetBit(a, to);
			move.moves[move.count++] = code_move(from, to);
			
		}
	}

	void ChessState::calculate_Pawn_Moves(MoveList& move)
	{

		U64 myPieces = MyPieces();
		U64 opponentPieces = OpponentPieces(); 
		if (move.count >= 256) {
			std::cout << "MoveList poza zakresem\n";
			std::cout << move.count;
			return;
		}
		
		if (white_move)
		{
			U64 singlePush = ((pieces[pawns] & myPieces) << 8) & emptySpaces;
			U64 doublePush = (singlePush << 8) & emptySpaces;


			while (singlePush != 0)
			{
				int to = std::countr_zero(singlePush);
				resetBit(singlePush, Square(to));
				if (move.count < 250) {
					move.moves[move.count++] = code_move(to - 8, to);
				}
				else {
					std::cout << "MoveList poza zakresem\n";
					return;
				}

			}
			while (doublePush != 0)
			{
				int to = std::countr_zero(doublePush);
				resetBit(doublePush, Square(to));
				if (move.count < 250) {
					move.moves[move.count++] = code_move(to -16, to);
				}
				else {
					std::cout << "MoveList poza zakresem\n";
					return;
				}
			}

			U64 leftPush = ((pieces[pawns] & myPieces) << 7) & opponentPieces;
			while (leftPush != 0)
			{
				int to = std::countr_zero(leftPush);
				resetBit(leftPush, Square(to));
				if (move.count < 250) {
					move.moves[move.count++] = code_move(to - 7, to);
				}
				else {
					std::cout << "MoveList overflow detected!\n";
					return;
				}
			}
			U64 rightPush = ((pieces[pawns] & myPieces) << 9) & opponentPieces;
			while (rightPush != 0)
			{
				int to = std::countr_zero(rightPush);
				resetBit(rightPush, Square(to));
				if (move.count < 250) {
					move.moves[move.count++] = code_move(to - 9, to);
				}
				else {
					std::cout << "MoveList overflow detected!\n";
					return;
				}
			}

		}
		else
		{

			U64 singlePush = ((pieces[pawns] & myPieces) >> 8) & emptySpaces;
			U64 doublePush = (singlePush >> 8) & emptySpaces;


			while (singlePush != 0)
			{
				int to = std::countr_zero(singlePush);
				resetBit(singlePush, Square(to));
				if (move.count < 250) {
					move.moves[move.count++] = code_move(to + 8, to);
				}
				else {
					std::cout << "MoveList overflow detected!\n";
					return;
				}


			}
			while (doublePush != 0)
			{
				int to = std::countr_zero(doublePush);
				resetBit(doublePush, Square(to));
				if (move.count < 250) {
					move.moves[move.count++] = code_move(to + 16, to);
				}
				else {
					std::cout << "MoveList overflow detected!\n";
					return;
				}
			}

			U64 leftPush = ((pieces[pawns] & myPieces) >> 9) & opponentPieces;
			while (leftPush != 0)
			{
				int to = std::countr_zero(leftPush);
				resetBit(leftPush, Square(to));
				if (move.count < 250) {
					move.moves[move.count++] = code_move(to + 9, to);
				}
				else {
					std::cout << "MoveList overflow detected!\n";
					return;
				}
			}
			U64 rightPush = ((pieces[pawns] & myPieces) >> 7) & opponentPieces;
			while (rightPush != 0)
			{
				int to = std::countr_zero(rightPush);
				resetBit(rightPush, Square(to));
				if (move.count < 250) {
					move.moves[move.count++] = code_move(to + 7, to);
				}
				else {
					std::cout << "MoveList overflow detected!\n";
					return;
				}
			}
		}
		if (move.count >= 256) {
			std::cout << "MoveList overflow detected!\n";
			std::cout << move.count;
			return;
		}
		/// tu pamiec jest okejjj 
	}

	U64 ChessState::calculate_bishop_moves(int square)
	{
		U64 myPieces = MyPieces();
		U64 opponentPieces = OpponentPieces();
		// U64 occupancy = myPieces | opponentPieces;
		U64 block;
		U64 attacks = 0ULL;
		//NE
		block = (NE[square] & myPieces);
		U64 NE_elminated = ~(block)&NE[square] & (~(NE[std::countr_zero(block)]));
		block = NE[square] & opponentPieces;
		NE_elminated &= NE[square] & (~(NE[std::countr_zero(block)]));
		//NW 
		block = (NW[square] & myPieces);
		U64 NW_elminated = ~(block)&NW[square] & (~(NW[std::countr_zero(block)]));
		block = NW[square] & opponentPieces;
		NW_elminated &= NW[square] & (~(NW[std::countr_zero(block)]));
		//SE
		block = (SE[square] & myPieces);
		U64 SE_elminated = ~(block)&SE[square] & (~(SE[63 - std::countl_zero(block)]));
		block = SE[square] & opponentPieces;
		SE_elminated &= SE[square] & (~(SE[63 - std::countl_zero(block)]));
		//SW
		block = (SW[square] & myPieces);
		U64 SW_elminated = ~(block)&SW[square] & (~(SW[63 - std::countl_zero(block)]));
		block = SW[square] & opponentPieces;
		SW_elminated &= SW[square] & (~(SW[63 - std::countl_zero(block)]));
		return SW_elminated | SE_elminated | NE_elminated | NW_elminated;
	}

	U64 ChessState::calculate_rook_moves(int square)
	{
		U64 myPieces = MyPieces();
		U64 opponentPieces = OpponentPieces();

		U64 attacks = 0ULL;
		U64 block;

		// NORTH
		block = North[square] & myPieces;
		U64 N_eliminated = ~(block)&North[square];
		if (block)
			N_eliminated &= ~(North[std::countr_zero(block)]);

		block = North[square] & opponentPieces;
		if (block)
			N_eliminated &= ~(North[std::countr_zero(block)]);

		// SOUTH
		block = South[square] & myPieces;
		U64 S_eliminated = ~(block)&South[square];
		if (block)
			S_eliminated &= ~(South[63 - std::countl_zero(block)]);

		block = South[square] & opponentPieces;
		if (block)
			S_eliminated &= ~(South[63 - std::countl_zero(block)]);

		// EAST
		block = East[square] & myPieces;
		U64 E_eliminated = ~(block)&East[square];
		if (block)
			E_eliminated &= ~(East[63 - std::countl_zero(block)]);

		block = East[square] & opponentPieces;
		if (block)
			E_eliminated &= ~(East[63 - std::countl_zero(block)]);

		// WEST
		block = West[square] & myPieces;
		U64 W_eliminated = ~(block)&West[square];
		if (block)
			W_eliminated &= ~(West[std::countr_zero(block)]);

		block = West[square] & opponentPieces;
		if (block)
			W_eliminated &= ~(West[std::countr_zero(block)]);
		U64 final = N_eliminated | S_eliminated | E_eliminated | W_eliminated;

		return final;
	}

	U64 ChessState::calculate_queen_moves(int sq)
	{
		return calculate_rook_moves(sq) | calculate_bishop_moves(sq);
	}

	U64 ChessState::calculate_Knight_moves(int a)
	{

		return Knights[a] & ~(MyPieces());
	}

	U64 ChessState::calculate_king_moves(int a)
	{
		return King[a] & (~(MyPieces()));
	}

	int ChessState::filter_king_captures(MoveList& move, MoveList& helper)
	{
		MoveList legal;
		legal.count = 0;

		for (int i = 0; i < move.count; ++i) {
			Move m = move.moves[i];
			Simulate_Move(m);
			helper.count = 0;
			generatePseudoLegalMoves(helper);

			bool illegal = false;
			for (int j = 0; j < helper.count; ++j) {
				int to = helper.moves[j] & 0x3f;
				if (getBit(pieces[kings] & OpponentPieces(), to))
				{
					illegal = true;
					break;
				}
			}

			undo_Move();

			if (!illegal)
				legal.moves[legal.count++] = m;
			if (legal.count > 250)
				std::cout << "filer-king b³ad w legal.count";
		}
		move = legal;
		if (move.count == 0)
		{
			checkhelper.count = 0;
			white_move = !white_move;
			generatePseudoLegalMoves(checkhelper);
			white_move = !white_move;
			
			for (int i = 0; i < checkhelper.count; i++)
			{
				int to = checkhelper.moves[i] & 0x3f;
				
				if (getBit((pieces[Chess::kings] & MyPieces()), to))
					return Mate;

			}
			return Pat;
		}
		return game;
	}
	U64 ChessState::row(int a)
	{
		U64 ret = 0ULL;
		for (int i = a * 8; i < a * 8 + 8; i++)
		{
			setBit(ret, (Square)(i));
		}
		return ret;
	}

	U64 ChessState::column(int a)
	{
		U64 ret = 0ULL;
		for (int i = a; i < 64; i += 8)
		{
			setBit(ret, (Square)(i));
		}
		return ret;
	}

	U64 ChessState::knightMoves(int square)
	{
		U64 bitboard = 0ULL;
		U64 knight = 1ULL << square;

		U64 moves = 0ULL;
		moves |= (knight << 17) & notAFile;
		moves |= (knight << 15) & notHFile;
		moves |= (knight << 10) & notABFile;
		moves |= (knight << 6) & notGHFile;
		moves |= (knight >> 17) & notHFile;
		moves |= (knight >> 15) & notAFile;
		moves |= (knight >> 10) & notGHFile;
		moves |= (knight >> 6) & notABFile;

		return moves;
	}

	void ChessState::generate_knight_table(U64(&Knights)[64])
	{
		for (int i = 0; i < 64; i++)
		{
			Knights[i] = knightMoves(i);
		}
	}

	void ChessState::generate_rook_tables(U64(&N_)[64], U64(&S_)[64], U64(&E_)[64], U64(&W_)[64])
	{
		for (int square = 0; square < 64; ++square) {
			int rank = square / 8;
			int file = square % 8;

			U64 N = 0ULL;
			U64 S = 0ULL;
			U64 E = 0ULL;
			U64 W = 0ULL;

			// North 
			for (int r = rank + 1; r < 8; ++r)
				N |= 1ULL << (r * 8 + file);

			// South 
			for (int r = rank - 1; r >= 0; --r)
				S |= 1ULL << (r * 8 + file);

			// East 
			for (int f = file + 1; f < 8; ++f)
				E |= 1ULL << (rank * 8 + f);

			// West 
			for (int f = file - 1; f >= 0; --f)
				W |= 1ULL << (rank * 8 + f);

			N_[square] = N;
			S_[square] = S;
			E_[square] = E;
			W_[square] = W;
		}
	}

	void ChessState::generate_bishop_tables(U64(&NE_)[64], U64(&NW_)[64], U64(&SE_)[64], U64(&SW_)[64])
	{
		for (int square = 0; square < 64; ++square) {
			int rank = square / 8;
			int file = square % 8;
			U64 NE = 0ULL;
			U64 SE = 0ULL;
			U64 NW = 0ULL;
			U64 SW = 0ULL;

			//  NE
			for (int r = rank + 1, f = file + 1; r < 8 && f < 8; ++r, ++f)
				NE |= 1ULL << (r * 8 + f);

			//  NW
			for (int r = rank + 1, f = file - 1; r < 8 && f >= 0; ++r, --f)
				NW |= 1ULL << (r * 8 + f);

			//  SE
			for (int r = rank - 1, f = file + 1; r >= 0 && f < 8; --r, ++f)
				SE |= 1ULL << (r * 8 + f);

			// SW
			for (int r = rank - 1, f = file - 1; r >= 0 && f >= 0; --r, --f)
				SW |= 1ULL << (r * 8 + f);

			NE_[square] = NE;
			NW_[square] = NW;
			SE_[square] = SE;
			SW_[square] = SW;
		}
	}

	void ChessState::generate_king_table(U64(&Kings)[64])
	{
		for (int square = 0; square < 64; square++)
		{
			U64 moves = 0ULL;
			int rank = square / 8;
			int file = square % 8;


			int dir[8][2] = {
				{1,0}, {-1,0}, {0,1}, {0,-1},
				{1,1}, {1,-1}, {-1,1}, {-1,-1}
			};

			for (auto& d : dir) {
				int r = rank + d[0];
				int f = file + d[1];
				if (r >= 0 && r < 8 && f >= 0 && f < 8) {
					moves |= (1ULL << (r * 8 + f));
				}
			}
			Kings[square] = moves;

		}



	}


}