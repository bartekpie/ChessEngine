#include "position.hpp"
Pieces Position::getCurrPiece(Bitboard::Square from) {
    Bitboard::bitboard from_bb = 1ULL << from ; 
    for (int piece{0}; piece < int(Pieces::size_of_pieces); piece++) {
       if (from_bb & board_[piece])
         return Pieces(piece);
    } 
    return Pieces::size_of_pieces;
    
}

void Position::simulate_move(Move move) {
    auto from = move.from();
    auto to = move.to();
    auto to_be_moved = getCurrPiece(from);
    auto doubledPushedMove = moreInfoManager_.last().doublePushedMove_;
    bool wasDoublePushed = false;
    Pieces captured {};
    switch (move.type()){
        case MoveType::standard : {
           Bitboard::reset_bit(board_[int(to_be_moved)], from);
           Bitboard::reset_bit(colorBoard_[int(sideToMove)], from);
           Bitboard::set_bit(emptySpaces_, from);

           Bitboard::set_bit(board_[int(to_be_moved)], to);
           Bitboard::set_bit(colorBoard_[int(sideToMove)], to);
           Bitboard::reset_bit(emptySpaces_, to);
           
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
            auto piece_on_to_square = getCurrPiece(to);

            Bitboard::reset_bit(board_[int(to_be_moved)], from);    
            Bitboard::reset_bit(colorBoard_[int(sideToMove)], from);
            Bitboard::set_bit(emptySpaces_, from);

            Bitboard::reset_bit(board_[int(piece_on_to_square)], to);    
            Bitboard::reset_bit(colorBoard_[int(sideToMove)], to);
            Bitboard::set_bit(emptySpaces_, to);

            if (to == Bitboard::a1) // long white castle
            {
                Bitboard::set_bit(board_[int(to_be_moved)], Bitboard::c1);
                Bitboard::set_bit(colorBoard_[int(sideToMove)], Bitboard::c1);
                Bitboard::reset_bit(emptySpaces_, Bitboard::c1);

                Bitboard::set_bit(board_[int(piece_on_to_square)], Bitboard::d1);
                Bitboard::set_bit(colorBoard_[int(sideToMove)], Bitboard::d1);
                Bitboard::reset_bit(emptySpaces_, Bitboard::d1);

                //reset long white castle
               
            }
            else if (to == Bitboard::h1) 
            {
                Bitboard::set_bit(board_[int(to_be_moved)], Bitboard::g1);
                Bitboard::set_bit(colorBoard_[int(sideToMove)], Bitboard::g1);
                Bitboard::reset_bit(emptySpaces_, Bitboard::g1);

                Bitboard::set_bit(board_[int(piece_on_to_square)], Bitboard::f1);
                Bitboard::set_bit(colorBoard_[int(sideToMove)], Bitboard::f1);
                Bitboard::reset_bit(emptySpaces_, Bitboard::f1);
            }
            else if (to == Bitboard::a8)
            {
                Bitboard::set_bit(board_[int(to_be_moved)], Bitboard::c8);
                Bitboard::set_bit(colorBoard_[int(sideToMove)], Bitboard::c8);
                Bitboard::reset_bit(emptySpaces_, Bitboard::c8);

                Bitboard::set_bit(board_[int(piece_on_to_square)], Bitboard::d8);
                Bitboard::set_bit(colorBoard_[int(sideToMove)], Bitboard::d8);
                Bitboard::reset_bit(emptySpaces_, Bitboard::d8);

            }
            else 
            {
                assert(to == Bitboard::h8);

                Bitboard::set_bit(board_[int(to_be_moved)], Bitboard::g8);
                Bitboard::set_bit(colorBoard_[int(sideToMove)], Bitboard::g8);
                Bitboard::reset_bit(emptySpaces_, Bitboard::g8);

                Bitboard::set_bit(board_[int(piece_on_to_square)], Bitboard::f8);
                Bitboard::set_bit(colorBoard_[int(sideToMove)], Bitboard::f8);
                Bitboard::reset_bit(emptySpaces_, Bitboard::f8);
            }
            break;

        }
        default : { // promotion
            break;
        }

    }
    sideToMove = sideToMove == Color::white ? Color::black : Color::white ;
}