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
    auto oppositeSide = sideToMove == Color::white ? Color::black : Color::white;
    Pieces captured {};
    MoreInfo moreinfo;
    moreinfo.movedPiece_ = to_be_moved;
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
                moreinfo.doublePushedMove_ = to;
           }
           break ;
        }
        case MoveType::capture : {
            captured = getCurrPiece(to);
            Bitboard::reset_bit(board_[int(to_be_moved)], from);
            Bitboard::set_bit(board_[int(to_be_moved)], to);
            Bitboard::reset_bit(board_[int(captured)], to);

            reset_bit(colorBoard_[int(oppositeSide)], to);
             
            reset_bit(colorBoard_[int(sideToMove)], from);
            set_bit(colorBoard_[int(sideToMove)], to);

            reset_bit(emptySpaces_, to);
            set_bit(emptySpaces_, from);

            moreinfo.capturedPiece_ = captured;
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

            moreinfo.afterPassantSquare_ = doubledPushedMove;
            moreinfo.capturedPiece_ = captured;

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

                moreinfo.castlingRights_ &= ~(white_queen_side);
               
            }
            else if (to == Bitboard::h1) 
            {
                Bitboard::set_bit(board_[int(to_be_moved)], Bitboard::g1);
                Bitboard::set_bit(colorBoard_[int(sideToMove)], Bitboard::g1);
                Bitboard::reset_bit(emptySpaces_, Bitboard::g1);

                Bitboard::set_bit(board_[int(piece_on_to_square)], Bitboard::f1);
                Bitboard::set_bit(colorBoard_[int(sideToMove)], Bitboard::f1);
                Bitboard::reset_bit(emptySpaces_, Bitboard::f1);

                moreinfo.castlingRights_ &= ~(white_king_side);
            }
            else if (to == Bitboard::a8)
            {
                Bitboard::set_bit(board_[int(to_be_moved)], Bitboard::c8);
                Bitboard::set_bit(colorBoard_[int(sideToMove)], Bitboard::c8);
                Bitboard::reset_bit(emptySpaces_, Bitboard::c8);

                Bitboard::set_bit(board_[int(piece_on_to_square)], Bitboard::d8);
                Bitboard::set_bit(colorBoard_[int(sideToMove)], Bitboard::d8);
                Bitboard::reset_bit(emptySpaces_, Bitboard::d8);

                moreinfo.castlingRights_ &= ~(black_queen_side);

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

                moreinfo.castlingRights_ &= ~(black_king_side);
            }
            break;

        }
        default : { // promotion
            if (std::abs(from - to) != 8) {
                captured = getCurrPiece(to);
                Bitboard::reset_bit(board_[int(captured)], to);
                Bitboard::reset_bit(colorBoard_[int(oppositeSide)], to);
                moreinfo.capturedPiece_ = captured;
            }
            Pieces promoted = Pieces::size_of_pieces;
            switch(move.type()) {
                case MoveType::promotionBishop :
                    promoted = sideToMove == Color::white ? Pieces::white_bishop : Pieces::black_bishop;
                    break;
                case MoveType::promotionKnight :
                    promoted = sideToMove == Color::white ? Pieces::white_knight : Pieces::black_knight;
                    break;
                case MoveType::promotionRook :
                    promoted = sideToMove == Color::white ? Pieces::white_rook : Pieces::black_rook;
                    break;
                case MoveType::promotionQueen :
                    promoted = sideToMove == Color::white ? Pieces::white_queen : Pieces::black_queen;
                    break;     
            }
            auto moving_pawns = sideToMove == Color::white ? Pieces::white_pawn : Pieces::black_pawn;
            Bitboard::reset_bit(board_[int(moving_pawns)],from);
            Bitboard::reset_bit(colorBoard_[int(sideToMove)], from);
            Bitboard::set_bit(board_[int(promoted)], to);
            Bitboard::set_bit(colorBoard_[int(sideToMove)], to);
            Bitboard::set_bit(emptySpaces_, from);
            Bitboard::reset_bit(emptySpaces_, to);
            break;
        }

    }
    sideToMove = sideToMove == Color::white ? Color::black : Color::white ;
    moreinfo.lastMove_ = move;
    moreInfoManager_.add(moreinfo);
}


void Position::undo_move()
{
    auto more_info = moreInfoManager_.pop();

    auto from = more_info.lastMove_.from();
    auto to = more_info.lastMove_.to();
    auto type = more_info.lastMove_.type();
    auto moved_piece = more_info.movedPiece_;
    auto captured_piece = more_info.capturedPiece_;
    auto after_passant_square = more_info.afterPassantSquare_;

    auto right_side = sideToMove == Color::white ? Color::black : Color::white;

    switch (type) {
        case MoveType::standard : {
            Bitboard::reset_bit(board_[int(moved_piece)], to);
            Bitboard::set_bit(board_[int(moved_piece)], from);

            Bitboard::set_bit(emptySpaces_, to);
            Bitboard::reset_bit(emptySpaces_, from);

            Bitboard::reset_bit(colorBoard_[int(right_side)], to);
            Bitboard::set_bit(colorBoard_[int(right_side)], from);

            break;
        }
        case MoveType::capture : {
            Bitboard::reset_bit(board_[int(moved_piece)], to);
            Bitboard::set_bit(board_[int(moved_piece)], from);

            Bitboard::reset_bit(emptySpaces_, from);
            Bitboard::reset_bit(emptySpaces_, to);

            Bitboard::set_bit(colorBoard_[int(sideToMove)], to);
            Bitboard::set_bit(colorBoard_[int(right_side)], from);
            Bitboard::reset_bit(colorBoard_[int(right_side)], to);

            assert(captured_piece != Pieces::size_of_pieces);

            Bitboard::set_bit(board_[int(captured_piece)], to);
            break;
        }
        case MoveType::passant : {
            Bitboard::reset_bit(board_[int(moved_piece)], to);
            Bitboard::set_bit(board_[int(moved_piece)], from);

            Bitboard::set_bit(emptySpaces_, to);
            Bitboard::reset_bit(emptySpaces_, from);

            Bitboard::reset_bit(colorBoard_[int(right_side)], to);
            Bitboard::set_bit(colorBoard_[int(right_side)], from);

            assert(captured_piece != Pieces::size_of_pieces);
            assert(after_passant_square != Bitboard::a1);

            Bitboard::set_bit(colorBoard_[int(sideToMove)], after_passant_square);
            Bitboard::reset_bit(emptySpaces_, after_passant_square);
            Bitboard::set_bit(board_[int(captured_piece)], after_passant_square);
            break;
        }
        case MoveType::castle : {
            
            if (to == Bitboard::a1) {
                Bitboard::set_bit(board_[int(Pieces::white_rook)], to);
                Bitboard::set_bit(colorBoard_[int(Color::white)], to);
                Bitboard::reset_bit(emptySpaces_, to);

                Bitboard::reset_bit(board_[int(Pieces::white_rook)], Bitboard::d1);
                Bitboard::reset_bit(colorBoard_[int(Color::white)], Bitboard::d1);
                Bitboard::set_bit(emptySpaces_, Bitboard::d1);

                Bitboard::reset_bit(board_[int(Pieces::white_king)], Bitboard::c1);
                Bitboard::reset_bit(colorBoard_[int(Color::white)], Bitboard::c1);
                Bitboard::set_bit(emptySpaces_, Bitboard::c1);

                Bitboard::set_bit(board_[int(Pieces::white_king)], from);
                Bitboard::set_bit(colorBoard_[int(Color::white)], from);
                Bitboard::reset_bit(emptySpaces_, from);
                break;
            }
            else if (to == Bitboard::h1)
            {
                Bitboard::set_bit(board_[int(Pieces::white_rook)], to);
                Bitboard::set_bit(colorBoard_[int(Color::white)], to);
                Bitboard::reset_bit(emptySpaces_, to);
                
                Bitboard::reset_bit(board_[int(Pieces::white_rook)], Bitboard::f1);
                Bitboard::reset_bit(colorBoard_[int(Color::white)], Bitboard::f1);
                Bitboard::set_bit(emptySpaces_, Bitboard::f1);

                Bitboard::reset_bit(board_[int(Pieces::white_king)], Bitboard::g1);
                Bitboard::reset_bit(colorBoard_[int(Color::white)], Bitboard::g1);
                Bitboard::set_bit(emptySpaces_, Bitboard::g1);

                Bitboard::set_bit(board_[int(Pieces::white_king)], from);
                Bitboard::set_bit(colorBoard_[int(Color::white)], from);
                Bitboard::reset_bit(emptySpaces_, from);
                break;
            }
            else if (to == Bitboard::a8) 
            {
                Bitboard::set_bit(board_[int(Pieces::black_rook)], to);
                Bitboard::set_bit(colorBoard_[int(Color::black)], to);
                Bitboard::reset_bit(emptySpaces_, to);

                Bitboard::reset_bit(board_[int(Pieces::black_rook)], Bitboard::d8);
                Bitboard::reset_bit(colorBoard_[int(Color::black)], Bitboard::d8);
                Bitboard::set_bit(emptySpaces_, Bitboard::d8);

                Bitboard::reset_bit(board_[int(Pieces::black_king)], Bitboard::c8);
                Bitboard::reset_bit(colorBoard_[int(Color::black)], Bitboard::c8);
                Bitboard::set_bit(emptySpaces_, Bitboard::c8);

                Bitboard::set_bit(board_[int(Pieces::black_king)], from);
                Bitboard::set_bit(colorBoard_[int(Color::black)], from);
                Bitboard::reset_bit(emptySpaces_, from);
                break;
            }
            else {
                assert(to == Bitboard::h8);

                Bitboard::set_bit(board_[int(Pieces::black_rook)], to);
                Bitboard::set_bit(colorBoard_[int(Color::black)], to);
                Bitboard::reset_bit(emptySpaces_, to);

                Bitboard::reset_bit(board_[int(Pieces::black_rook)], Bitboard::f8);
                Bitboard::reset_bit(colorBoard_[int(Color::black)], Bitboard::f8);
                Bitboard::set_bit(emptySpaces_, Bitboard::f8);

                Bitboard::reset_bit(board_[int(Pieces::black_king)], Bitboard::g8);
                Bitboard::reset_bit(colorBoard_[int(Color::black)], Bitboard::g8);
                Bitboard::set_bit(emptySpaces_, Bitboard::g8);

                Bitboard::set_bit(board_[int(Pieces::black_king)], from);
                Bitboard::set_bit(colorBoard_[int(Color::black)], from);
                Bitboard::reset_bit(emptySpaces_, from);
                
                break;
            } 
        }  
        default : {
            Pieces promoted = Pieces::size_of_pieces;
            switch(type) {
                case MoveType::promotionBishop :
                    promoted = right_side == Color::white ? Pieces::white_bishop : Pieces::black_bishop;
                    break;
                case MoveType::promotionKnight :
                    promoted = right_side == Color::white ? Pieces::white_knight : Pieces::black_knight;
                    break;
                case MoveType::promotionRook :
                    promoted = right_side == Color::white ? Pieces::white_rook : Pieces::black_rook;
                    break;
                case MoveType::promotionQueen :
                    promoted = right_side == Color::white ? Pieces::white_queen : Pieces::black_queen;
                    break;     
            }
            
            Bitboard::reset_bit(board_[int(promoted)], to);
            Bitboard::reset_bit(colorBoard_[int(right_side)], to); 

            auto moving_pawns = right_side == Color::white ? Pieces::white_pawn : Pieces::black_pawn;
            
            Bitboard::reset_bit(emptySpaces_, from);
            Bitboard::set_bit(board_[int(moving_pawns)], from);
            Bitboard::set_bit(colorBoard_[int(right_side)], from);
            
            if (std::abs(to-from) != 8) {
                Bitboard::set_bit(colorBoard_[int(sideToMove)], to);
                Bitboard::set_bit(board_[int(captured_piece)], to);
                Bitboard::reset_bit(emptySpaces_, to);
            }
            else {
                Bitboard::set_bit(emptySpaces_, to);
            }
        } 
         
    }
    sideToMove = sideToMove == Color::white ? Color::black : Color::white; 
}
