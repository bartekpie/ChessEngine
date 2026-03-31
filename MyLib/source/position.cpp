#include "position.hpp"
Pieces Position::getCurrPiece(Bitboard::Square from) {
    Bitboard::bitboard from_bb = 1ULL << from ; 
    for (int piece{0}; piece < int(Pieces::size_of_pieces); piece++) {
       if (Bitboard::lsb(from_bb & board_[piece]) == from)
         return Pieces(piece);
    } 
    
}

void Position::simulate_move(Move move) {
    auto from = move.from();
    auto to = move.to();
    auto to_be_moved = getCurrPiece(from);
    auto doubledPushedMove = moreInfoManager_.pop().doublePushedMove_;
    bool wasDoublePushed = false;
    Pieces captured {};
    switch (move.type()){
        case MoveType::standard : {
           Bitboard::reset_bit(board_[int(to_be_moved)], from);
           Bitboard::set_bit(board_[int(to_be_moved)], to);
           reset_bit(colorBoard_[int(sideToMove)], from);
           set_bit(colorBoard_[int(sideToMove)], to);
           reset_bit(emptySpaces_, to);
           set_bit(emptySpaces_, from);
           if (to_be_moved == Pieces::black_pawn || to_be_moved == Pieces::white_pawn) {
              if(std::abs(from - to) == 16)
                wasDoublePushed = true;
           }
           break ;
        }
        case MoveType::capture : {
            captured = getCurrPiece(to);
            Bitboard::reset_bit(board_[int(to_be_moved)], from);
            Bitboard::set_bit(board_[int(to_be_moved)], to);
            Bitboard::reset_bit(board_[int(captured)], to);

            auto oppositeSide = sideToMove == Color::white ? Color::black : Color::white;
            reset_bit(colorBoard_[int(oppositeSide)], to);
             
            reset_bit(colorBoard_[int(sideToMove)], from);
            set_bit(colorBoard_[int(sideToMove)], to);

            reset_bit(emptySpaces_, to);
            set_bit(emptySpaces_, from);
            break ;
        }
        case MoveType::passant : {
            assert(doubledPushedMove != Bitboard::Square::a1);
            assert(getCurrPiece(doubledPushedMove) == Pieces::black_pawn || getCurrPiece(doubledPushedMove) == Pieces::white_pawn);
            captured = getCurrPiece(doubledPushedMove); 
            Bitboard::reset_bit(board_[int(to_be_moved)], from);    
            Bitboard::reset_bit(colorBoard_[int(sideToMove)], from);
            Bitboard::set_bit(emptySpaces_, from);

            Bitboard::set_bit(board_[int(to_be_moved)], to);     
            Bitboard::set_bit(colorBoard_[int(sideToMove)], to);  
            Bitboard::reset_bit(emptySpaces_, to);

            Bitboard::reset_bit(board_[int(captured)], doubledPushedMove);
            Bitboard::reset_bit(colorBoard_[int((sideToMove == Color::white ? Color::black : Color::white))], doubledPushedMove);
            Bitboard::set_bit(emptySpaces_, doubledPushedMove);
            
            break;
            
        }
        case MoveType::castle : {


        }
        default : {
            
        }

    }
}