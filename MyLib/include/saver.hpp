#include<iostream>
#include <string>
#include<queue>
#include <array>
#include"chesstate.hpp"
#include<fstream>
class Saver {
public:
	
	int strToint(std::string& s);
	std::string createString(Chess::Undo undo);
	bool saveInFile(std::string text);
	void fromFiletoQueue();
	std::queue<Chess::Move> getQueue();
	
private:
	int movescount{ 0 };
	std::queue<Chess::Move> todomoves;
	std::array<char, 6> names = {
		'P', 'K', 'B', 'R', 'Q', 'K'
	};
	std::string fromIndex(int index);
	
};