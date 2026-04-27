#include <gtest/gtest.h>
#include <algorithm>
#include "generator.hpp"
#include "position.hpp"
TEST(LegalMoves, kingCannotCaptureAttackedPiece) {
    Bitboard::print_bitboard(123); // for debuging during gdb session
    const std::string pos = "7k/8/8/3r4/4K3/3r4/8/8 w - - 0 1";
    MoveList list;
    Position position(pos);
    generate_all_moves(position, list); 
    EXPECT_EQ(list.size(), 1);
     auto has_move = [&](Bitboard::Square from, Bitboard::Square to, MoveType type) {
        for (const auto& m : list) if (m.from() == from && m.to() == to && m.type() == type) return true;
        return false;
    };

    EXPECT_TRUE(has_move(Bitboard::e4, Bitboard::f4, standard));
};
TEST(LegalMoves, kingCannotMoveIntoAttackingSquare) {
    const std::string pos = "7k/8/8/3r4/4K3/8/8/8 w - - 0 1";
    MoveList list;
    Position position(pos);
    generate_all_moves(position, list); 
    EXPECT_EQ(list.size(), 4);
     auto has_move = [&](Bitboard::Square from, Bitboard::Square to, MoveType type) {
        for (const auto& m : list) if (m.from() == from && m.to() == to && m.type() == type) return true;
        return false;
    };

    EXPECT_TRUE(has_move(Bitboard::e4, Bitboard::d5, capture));
    EXPECT_TRUE(has_move(Bitboard::e4, Bitboard::e3, standard));
    EXPECT_TRUE(has_move(Bitboard::e4, Bitboard::f3, standard));
    EXPECT_TRUE(has_move(Bitboard::e4, Bitboard::f4, standard));
};
TEST(LegalMoves, pinnedPieceCannotMove) {
    const std::string pos = "4k3/8/8/8/8/4r3/4B3/4K3 w - - 0 1";
    MoveList list;
    Position position(pos);
    generate_all_moves(position, list);

    EXPECT_EQ(list.size(), 4);
     auto has_move = [&](Bitboard::Square from, Bitboard::Square to, MoveType type) {
        for (const auto& m : list) if (m.from() == from && m.to() == to && m.type() == type) return true;
        return false;
    };

    EXPECT_TRUE(has_move(Bitboard::e1, Bitboard::d1, standard));
    EXPECT_TRUE(has_move(Bitboard::e1, Bitboard::d2, standard));
    EXPECT_TRUE(has_move(Bitboard::e1, Bitboard::f1, standard));
    EXPECT_TRUE(has_move(Bitboard::e1, Bitboard::f2, standard));
};
TEST(LegalMoves, pinnedQueenCanOnlyMoveAlongPin) {
    const std::string pos = "4k3/8/8/8/4r3/8/4Q3/4K3 w - - 0 1";
    MoveList list;
    Position position(pos);
    generate_all_moves(position, list);

    EXPECT_EQ(list.size(), 6);

    auto has_move = [&](Bitboard::Square from, Bitboard::Square to, MoveType type) {
        for (const auto& m : list)
            if (m.from() == from && m.to() == to && m.type() == type) return true;
        return false;
    };

    EXPECT_TRUE(has_move(Bitboard::e2, Bitboard::e3, standard));
    EXPECT_TRUE(has_move(Bitboard::e2, Bitboard::e4, capture));
    EXPECT_TRUE(has_move(Bitboard::e1, Bitboard::d1, standard));
    EXPECT_TRUE(has_move(Bitboard::e1, Bitboard::d2, standard));
    EXPECT_TRUE(has_move(Bitboard::e1, Bitboard::f1, standard));
    EXPECT_TRUE(has_move(Bitboard::e1, Bitboard::f2, standard));
};
TEST(LegalMoves, kingInCheckOnlyEscapeMoves) {
    const std::string pos = "4k3/8/8/8/8/8/4R3/4K3 b - - 0 1";
    MoveList list;
    Position position(pos);
    generate_all_moves(position, list);

    EXPECT_EQ(list.size(), 4);

    auto has_move = [&](Bitboard::Square from, Bitboard::Square to, MoveType type) {
        for (const auto& m : list)
            if (m.from() == from && m.to() == to && m.type() == type) return true;
        return false;
    };

    EXPECT_TRUE(has_move(Bitboard::e8, Bitboard::d8, standard));
    EXPECT_TRUE(has_move(Bitboard::e8, Bitboard::f8, standard));
    EXPECT_TRUE(has_move(Bitboard::e8, Bitboard::d7, standard));
    EXPECT_TRUE(has_move(Bitboard::e8, Bitboard::f7, standard));
};
TEST(LegalMoves, kingCanCaptureCheckingPiece) {
    const std::string pos = "4k3/8/8/8/8/8/4r3/4K3 w - - 0 1";
    MoveList list;
    Position position(pos);
    generate_all_moves(position, list);

    EXPECT_EQ(list.size(), 3);

    auto has_move = [&](Bitboard::Square from, Bitboard::Square to, MoveType type) {
        for (const auto& m : list)
            if (m.from() == from && m.to() == to && m.type() == type) return true;
        return false;
    };

    EXPECT_TRUE(has_move(Bitboard::e1, Bitboard::e2, capture));
    EXPECT_TRUE(has_move(Bitboard::e1, Bitboard::d1, standard));
    EXPECT_TRUE(has_move(Bitboard::e1, Bitboard::f1, standard));
};
TEST(LegalMoves, scholarsMateIsCheckmate) {
    const std::string pos = "r1bqkb1r/pppp1Qpp/2n2n2/4p3/2B1P3/8/PPPP1PPP/RNB1K1NR b KQkq - 0 4";
    
    MoveList list;
    Position position(pos);
    generate_all_moves(position, list);


    EXPECT_EQ(list.size(), 0);
    
};
TEST(LegalMoves, checkPinnedPawns) {
    const std::string pos = "rnbqkbnr/pppppppp/8/8/Q7/8/PP1PPPPP/RNB1KBNR b KQkq - 0 4";
    
    MoveList list;
    Position position(pos);
    generate_all_moves(position, list);
    
    auto has_move = [&](Bitboard::Square from, Bitboard::Square to, MoveType type) {
        for (const auto& m : list)
            if (m.from() == from && m.to() == to && m.type() == type) return true;
        return false;
    };

    EXPECT_FALSE(has_move(Bitboard::d7, Bitboard::d6, standard));
    
};
TEST(LegalMoves, checkPinnedPawnsBlack) {
    const std::string pos = "rnb1kbnr/1p1ppppp/p1p5/q7/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 4";
    
    MoveList list;
    Position position(pos);
    generate_all_moves(position, list);
    
    auto has_move = [&](Bitboard::Square from, Bitboard::Square to, MoveType type) {
        for (const auto& m : list)
            if (m.from() == from && m.to() == to && m.type() == type) return true;
        return false;
    };

    EXPECT_FALSE(has_move(Bitboard::d2, Bitboard::d3, standard));
    
};
TEST(LegalMoves, singleCheck) {
    const std::string pos = "rnb1kbnr/1p1ppppp/p1p5/q7/8/3P4/PPP1PPPP/RNBQKBNR w KQkq - 0 4";
    
    MoveList list;
    Position position(pos);
    generate_all_moves(position, list);
    
    auto has_move = [&](Bitboard::Square from, Bitboard::Square to, MoveType type) {
        for (const auto& m : list)
            if (m.from() == from && m.to() == to && m.type() == type) return true;
        return false;
    };

    EXPECT_EQ(list.size(), 5);
    EXPECT_TRUE(has_move(Bitboard::d1, Bitboard::d2, standard));
    EXPECT_TRUE(has_move(Bitboard::c1, Bitboard::d2, standard));
    EXPECT_TRUE(has_move(Bitboard::c2, Bitboard::c3, standard));
    EXPECT_TRUE(has_move(Bitboard::b1, Bitboard::d2, standard));
    EXPECT_TRUE(has_move(Bitboard::b1, Bitboard::c3, standard));
    
};
TEST(LegalMoves, kingCannotGoInAttackUnderCheck) {
    const std::string pos = "k7/8/8/q7/8/8/3K4/8 w KQkq - 0 4";
    
    MoveList list;
    Position position(pos);
    generate_all_moves(position, list);
    
    auto has_move = [&](Bitboard::Square from, Bitboard::Square to, MoveType type) {
        for (const auto& m : list)
            if (m.from() == from && m.to() == to && m.type() == type) return true;
        return false;
    };

    EXPECT_EQ(list.size(), 6);
    EXPECT_TRUE(has_move(Bitboard::d2, Bitboard::c2, standard));
    EXPECT_TRUE(has_move(Bitboard::d2, Bitboard::c1, standard));
    EXPECT_TRUE(has_move(Bitboard::d2, Bitboard::d1, standard));
    EXPECT_TRUE(has_move(Bitboard::d2, Bitboard::e2, standard));
    EXPECT_TRUE(has_move(Bitboard::d2, Bitboard::e3, standard));
    EXPECT_TRUE(has_move(Bitboard::d2, Bitboard::d3, standard));
    
};
TEST(LegalMoves, queenMovesHaveToStop) {
    const std::string pos = "k1b4Q/8/8/8/8/8/3K4/8 w KQkq - 0 4";
    
    MoveList list;
    Position position(pos);
    generate_all_moves(position, list);
    
    auto has_move = [&](Bitboard::Square from, Bitboard::Square to, MoveType type) {
        for (const auto& m : list)
            if (m.from() == from && m.to() == to && m.type() == type) return true;
        return false;
    };

    EXPECT_TRUE(has_move(Bitboard::h8, Bitboard::c8, capture));
    EXPECT_FALSE(has_move(Bitboard::h8, Bitboard::a8, capture));
}