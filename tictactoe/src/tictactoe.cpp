#include "tictactoe/tictactoe.h"

/**
* Default constructor for the player class. 
*/
player::player() {
	mType = COMPUTER;
}

/**
* Base class for a player. 
* @param type the player type, USER or COMPUTER
*/
player::player(player_type type) {
	mType = type;
}

/**
* Returns the player's playertype. 
* @return player_type the players type. 
*/
player_type player::getPlayerType() {
	return mType;
}

/**
* Sets the players playertype. 
* @param type the new player_type. 
*/
void player::setPlayerType(player_type type) {
	mType = type;
}

/**
* Computer player. AI, extends player. 
*/
ComputerPlayer::ComputerPlayer(player_type type) : player(type) {
	depth = 0;
}

/**
* Perfoms a player_move on the current board. 
* @param board the current board. 
*/
void ComputerPlayer::performMove(Board &board) {
	depth = 0;
	ComputerMove move = getBestMove(board, new player(COMPUTER));
	board.addAPlay(move.p, new player(COMPUTER));
}

/**
* Set the difficulty of the computer player. This can be between 0 and 3.
* @param diff the difficulty. 
*/
void ComputerPlayer::setDifficulty(int diff) {
	int random = 0;
	switch (diff) {
	case 0:
		maxDepth = 1;
		random = rand() % 90 + 87;
		mScore = 100 + random;
		mOppScore = 100 - random;
		break;
	case 1:
		maxDepth = 2;
		random = rand() % 80 + 70;
		mScore = 100 + random;
		mOppScore = 100 - random;
		break;
	case 2:
		maxDepth = 5;
		random = rand() % 50 + 20;
		mScore = 100 + random;
		mOppScore = 100 - random;
		break;
	case 3:
		maxDepth = 8;
		mScore = 100;
		mOppScore = 100;
		break;
	}
}

/**
* Recursive function that gets the best player_move for the computer. Uses the 
* minimax algorithm to go through all possible outcomes assuming the oppenent will
* try to maximize his score. The computer will try to maximize his score as well. 
* @param board the current board. 
* @param player pointer to who is making the player_move. 
* @return ComputerMove a player_move with a point and a score. 
*/
ComputerMove ComputerPlayer::getBestMove(Board& board, player *player) {
	
	//base case, check for end state. 
	if (board.hasOWon()) {
		//computer won. User is always x.
		return ComputerMove(mScore);
	}
	else if (board.hasXWon()) {
		//computer lost, he doesn't like this. 
		return ComputerMove(-1*(mOppScore));
	}
	else if (board.isGameOver()) {
		//tie.
		return ComputerMove(0);
	}

	//available points. 
	std::vector<Point*> availablePoints = board.getAvailablePoints();
	//vector to hold all the moves we make to look through them later. 
	std::vector<ComputerMove> moves;
	
	//go through moves. 
	for (int i = 0; i < availablePoints.size(); i++) {
		ComputerMove move;
		move.p = *availablePoints.at(i);
		//add player_move. 
		board.addAPlay(*availablePoints.at(i), player);
		//recursive call. 
		if (player->getPlayerType() == USER) {
			move.score = getBestMove(board, new player(COMPUTER)).score;
		}
		else if (player->getPlayerType() == COMPUTER) {
			move.score = getBestMove(board, new player(USER)).score;
		}
		//remember the player_move we made. 
		moves.push_back(move);
		//remove the player_move from the board. 
		board.removePlay(*availablePoints.at(i));
	}


	//Pick best player_move for the current player.
	int bestMove = 0;
	if (player->getPlayerType() == COMPUTER) {
		int bestScore = -100000;
		for (int x = 0; x < moves.size(); x++) {
			if (moves[x].score > bestScore) {
				bestMove = x;
				bestScore = moves[x].score;
			}
		}
	}
	else if (player->getPlayerType() == USER) {
		int bestScore = 100000;
		for (int x = 0; x < moves.size(); x++) {
			if (moves[x].score < bestScore) {
				bestMove = x;
				bestScore = moves[x].score;
			}
		}
	}

	//return the best player_move. 
	return moves[bestMove];
}