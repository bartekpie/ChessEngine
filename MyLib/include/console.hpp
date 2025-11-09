#pragma once
#include<iostream>
#include"chesstate.hpp"

#include"const.hpp"

int square_from_string(std::string str);

Chess::Move parse_move(std::string input);

void handle_move_input(std::string input,Chess::ChessState &state);

void setLetter(U64 wzor, char symbol, char board[64]);

void print_board(Chess::ChessState state, char board[64]);
