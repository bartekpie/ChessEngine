#include<iostream>
#include<queue>
#include"chesstate.hpp"
class Saver {
public:
	int strToint(std::string& s);
	std::string createString(Chess::Undo undo);
	bool saveInFile(std::string text);
	void fromFiletoQueue();
private:
	int movescount{ 0 };
	std::queue<Move> todomoves;
	std::array<char, 6> names = {
		'P', 'K', 'B', 'R', 'Q', 'K'
	};
	std::string fromIndex(int index);
	
};