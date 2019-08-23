#ifndef TICTACTOE_H
#define TICTACTOE_H

#include <tictactoe/point_t.h>

#include <vector>


class Board;

struct ComputerMove {
	ComputerMove() {};
	ComputerMove(int s){ score = s; };
	Point p;
	int score;
};

class ComputerPlayer : public player {
public:
	ComputerPlayer(player_type type = COMPUTER);
	void performMove(Board &board);
	void setDifficulty(int diff);
private:
	int depth, maxDepth = 1000;
	int mScore, mOppScore;
	ComputerMove getBestMove(Board& board, player *player);
};

#endif