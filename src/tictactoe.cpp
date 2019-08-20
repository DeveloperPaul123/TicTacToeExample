#include "tictactoe.h"

#include <QMouseEvent>

/**
* Default constructor. 
*/
Point::Point() {

}

/**
* Constructor with x and y coordinate. 
* @param x x coordinate. 
* @param y y coordinate. 
*/
Point::Point(int x, int y) {
	mX = x;
	mY = y;
}

/**
* Returns the x coordinate. 
* @return int the x coordinate. 
*/
int Point::getX() {
	return mX;
}

/**
* Returns the y coordinate. 
* @return int the y coordinate. 
*/
int Point::getY() {
	return mY;
}

/**
* Converts the point to a qstring. 
* @return QString the point in QString form. 
*/
QString Point::toString() {
	return QString("[%1, %2]").arg(mX, mY);
}

/**
* Compares the current point to a given point. 
* @param p the point to compare to. 
* @return bool true if the points are equal, false otherwise. 
*/
bool Point::equals(Point p) {
	return (p.getX() == mX && p.getY() == mY);
}

/**
* Default constructor for the player class. 
*/
Player::Player() {
	mType = COMPUTER;
}

/**
* Base class for a player. 
* @param type the player type, USER or COMPUTER
*/
Player::Player(PlayerType type) {
	mType = type;
}

/**
* Returns the player's playertype. 
* @return PlayerType the players type. 
*/
PlayerType Player::getPlayerType() {
	return mType;
}

/**
* Sets the players playertype. 
* @param type the new PlayerType. 
*/
void Player::setPlayerType(PlayerType type) {
	mType = type;
}

/**
* Widget to display a tic tac toe board. 
* @param parent the QWidget parent of this widget. 
*/
BoardWidget::BoardWidget(QWidget* parent) : QWidget(parent) {
	setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
	resize(sizeHint());
	int thirdWidth = width() / 3;
	int thirdHeight = height() / 3;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			board[i][j] = 0;
			rects[i][j] = new QRect(j*thirdWidth, i*thirdHeight, thirdWidth, thirdHeight);
		}
	}

}

/**
* Updates the board based on the played squares of the board. 
* @param b the current Board. 
*/
void BoardWidget::updateBoard(Board &b) {
	//update the board based on current play. 
	std::vector<Point*> points = b.getPlayedPoints();
	for (int i = 0; i < points.size(); i++) {
		int x = points.at(i)->getX();
		int y = points.at(i)->getY();

		PlayerType player = b.readBoard(*points.at(i));
		board[x][y] = player;
	}

	update();
}

/**
* Called when the widget draws itself. 
* @param event the QPaintEvent passed to this widget. 
*/
void BoardWidget::paintEvent(QPaintEvent *event) {
	QPainter painter(this);
	painter.setBrush(Qt::NoBrush);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(QPen(QColor(0, 0, 0), 2.0, Qt::SolidLine, Qt::RoundCap, Qt::BevelJoin));

	int thirdWidth = width() / 3;
	int thirdHeight = height() / 3;

	//draw the actual board. 
	painter.drawLine(thirdWidth, 0.0, thirdWidth, height());
	painter.drawLine(2 * thirdWidth, 0.0, 2 * thirdWidth, height());
	painter.drawLine(0.0, thirdHeight, width(), thirdHeight);
	painter.drawLine(0.0, 2 * thirdHeight, width(), 2 * thirdHeight);
	
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			rects[i][j] = new QRect(j*thirdWidth + 3, i*thirdHeight +3 , 
				thirdWidth-3, thirdHeight-3);
		}
	}

	painter.setPen(QPen(QColor(51, 102, 204), 6.0, Qt::SolidLine, Qt::RoundCap));
	//draw x's and o's
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (board[i][j] == USER) {
				//user is X.
				QPainterPath path = getXPath(i, j);
				painter.drawPath(path);
			} 
			else if (board[i][j] == COMPUTER) {
				QPainterPath path = getOPath(i, j);		
				painter.drawPath(path);
			}
		}
	}

}

/**
* Clears everything on the board. 
*/
void BoardWidget::resetBoard() {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			board[i][j] = 0;
		}
	}
	update();
}

/**
* Helper function to get an "X" to draw. 
* @param row which row of the board we're drawing on.
* @param col which column of the board we're drawing on. 
* @return QPainterPath a path that can be drawn by the painter. 
*/
QPainterPath BoardWidget::getXPath(int row, int col) {
	QPainterPath *path = new QPainterPath();
	int thirdHeight = height() / 3;
	int thirdWidth = width() / 3;

	//define a bounding rect. 
	QRect rect = *rects[row][col];
	int padding = 15;
	path->moveTo(rect.left()+10, rect.top()+10);
	path->lineTo(rect.right()-10, rect.bottom()-10);
	path->moveTo(rect.right()-10, rect.top()+10);
	path->lineTo(rect.left()+10, rect.bottom()-10);

	return *path;
}

/**
* Helper function to get an "O" to draw. 
* @param row which row of the board we're drawing on. 
* @param col which column of the board we're drawing on. 
* @return QPainterPath the path that can be drawn by the painter. 
*/
QPainterPath BoardWidget::getOPath(int row, int col) {
	QPainterPath *path = new QPainterPath();
	int thirdHeight = height() / 3;
	int thirdWidth = width() / 3;

	//define a bounding rect. 
	QRect rect = *rects[row][col];
	int padding = 15;
	qreal radius = qMin(rect.width()/2, rect.height()/2);
	radius -= padding;
	path->addEllipse(rect.center(), radius, radius);
	return *path;
}

/**
* Size hint for the widget. Used when being laid out. 
* @return QSize the size we want for the widget. 
*/
QSize BoardWidget::sizeHint() const {
	return QSize(100, 100);
}

/**
* Called when the user clicks the board. 
* @param event the QMouseEvent passed to the widget. 
*/
void BoardWidget::mousePressEvent(QMouseEvent *event) {
	int x = event->x();
	int y = event->y();
	//check where they clicked (i.e. which square on the board.
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (rects[i][j]->contains(x, y)) {
				emit userPlayed(Point(i, j));
				break;
			}
		}
	}
}

/**
* Class that holds the current board and where each player has played. 
*/
Board::Board() {

	//initialize the board and vectors. 
	availablePoints = new std::vector<Point*>();
	playedPoints = new std::vector<Point*>();

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			board[i][j] = NO_ONE;
			availablePoints->push_back(new Point(i, j));
		}
	}
}

/**
* Reads the board at a specified point. 
* @param p the point to look at. 
* @return PlayerType the contents of the board at the given point. 
*/
PlayerType Board::readBoard(Point p) {

	if (board[p.getX()][p.getY()] == (int) USER) {
		return USER;
	}
	else if (board[p.getX()][p.getY()] == (int) COMPUTER) {
		return COMPUTER;
	}
	else {
		return NO_ONE;
	}
}

/**
* Checks if the game is over. A game is over if x has one, o has won or there is a tie. 
* @return bool true if game is over. False otherwise. 
*/
bool Board::isGameOver() {
	return (hasXWon() || hasOWon() || availablePoints->size() == 0);
}

/**
* Checks to see if X has won. 
* @return bool true if x has won, false otherwise. 
*/
bool Board::hasXWon() {
	if ((board[0][0] == board[1][1] && board[0][0] == board[2][2] && board[0][0] == USER) 
		|| (board[0][2] == board[1][1] && board[0][2] == board[2][0] && board[0][2] == USER)) {
		return true;
	}
	for (int i = 0; i < 3; ++i) {
		if ((board[i][0] == board[i][1] && board[i][0] == board[i][2] && board[i][0] == USER)
			|| (board[0][i] == board[1][i] && board[0][i] == board[2][i] && board[0][i] == USER)) {
			return true;
		}
	}

	return false;
}

/**
* Checks to see if O has won. 
* @return bool true if O has won, false otherwise. 
*/
bool Board::hasOWon() {
	if ((board[0][0] == board[1][1] && board[0][0] == board[2][2] && board[0][0] == COMPUTER)
		|| (board[0][2] == board[1][1] && board[0][2] == board[2][0] && board[0][2] == COMPUTER)) {
		return true;
	}
	for (int i = 0; i < 3; ++i) {
		if ((board[i][0] == board[i][1] && board[i][0] == board[i][2] && board[i][0] == COMPUTER)
			|| (board[0][i] == board[1][i] && board[0][i] == board[2][i] && board[0][i] == COMPUTER)) {
			return true;
		}
	}

	return false;
}

/**
* Returns the current vector of played points. 
* @return vector<Point*> the current vector of points. 
*/
std::vector<Point*> Board::getPlayedPoints() {
	return *playedPoints;
}

/**
* Returns a vector of available points. 
* @return vector<Point*> a vector of available points. 
*/
std::vector<Point*> Board::getAvailablePoints() {
	return *availablePoints;
}

/**
* Adds a play to the current board. 
* @param play the play point. 
* @param player pointer to the player making the play. 
* @return bool true if the play was made, false otherwise. 
*/
bool Board::addAPlay(Point play, Player *player) {
	for (int i = 0; i < availablePoints->size(); i++) {
		Point *p = availablePoints->at(i);
		if (p->equals(play)){
			availablePoints->erase(availablePoints->begin() + i);
			playedPoints->push_back(new Point(play));
			if (player->getPlayerType() == USER) {
				board[p->getX()][p->getY()] = (int)USER;
			}
			else if (player->getPlayerType() == COMPUTER) {
				board[p->getX()][p->getY()] = (int)COMPUTER;
			}
			return true;
		}
	}

	return false;
}

/**
* Removes a play from the board. 
* @param play the play to remove from the board. 
*/
void Board::removePlay(Point play) {
	board[play.getX()][play.getY()] = 0;
	availablePoints->push_back(new Point(play));
	for (int i = 0; i < playedPoints->size(); i++) {
		Point *p = playedPoints->at(i);
		if (p->equals(play)){
			playedPoints->erase(playedPoints->begin() + i);
			break;
		}
	}
}

/**
* Resets everything. Clears the board. 
*/
void Board::clearBoard() {
	availablePoints->clear();
	availablePoints = new std::vector<Point*>();
	playedPoints->clear();
	playedPoints = new std::vector<Point*>();
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			board[i][j] = NO_ONE;
			availablePoints->push_back(new Point(i, j));
		}
	}
}

/**
* Computer player. AI, extends player. 
*/
ComputerPlayer::ComputerPlayer(PlayerType type) : Player(type) {
	depth = 0;
}

/**
* Perfoms a move on the current board. 
* @param board the current board. 
*/
void ComputerPlayer::performMove(Board &board) {
	depth = 0;
	ComputerMove move = getBestMove(board, new Player(COMPUTER));
	board.addAPlay(move.p, new Player(COMPUTER));
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
* Recursive function that gets the best move for the computer. Uses the 
* minimax algorithm to go through all possible outcomes assuming the oppenent will
* try to maximize his score. The computer will try to maximize his score as well. 
* @param board the current board. 
* @param player pointer to who is making the move. 
* @return ComputerMove a move with a point and a score. 
*/
ComputerMove ComputerPlayer::getBestMove(Board& board, Player *player) {
	
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
		//add move. 
		board.addAPlay(*availablePoints.at(i), player);
		//recursive call. 
		if (player->getPlayerType() == USER) {
			move.score = getBestMove(board, new Player(COMPUTER)).score;
		}
		else if (player->getPlayerType() == COMPUTER) {
			move.score = getBestMove(board, new Player(USER)).score;
		}
		//remember the move we made. 
		moves.push_back(move);
		//remove the move from the board. 
		board.removePlay(*availablePoints.at(i));
	}


	//Pick best move for the current player.
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

	//return the best move. 
	return moves[bestMove];
}