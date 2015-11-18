#include "TicTacToe.h"

Point::Point() {

}

Point::Point(int x, int y) {
	mX = x;
	mY = y;
}

int Point::getX() {
	return mX;
}

int Point::getY() {
	return mY;
}

QString Point::toString() {
	return QString("[%1, %2]").arg(mX, mY);
}

bool Point::equals(Point p) {
	return (p.getX() == mX && p.getY() == mY);
}

Player::Player() {
	mType = COMPUTER;
}

Player::Player(PlayerType type) {
	mType = type;
}

PlayerType Player::getPlayerType() {
	return mType;
}

void Player::setPlayerType(PlayerType type) {
	mType = type;
}

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

void BoardWidget::updateBoard(Board &b) {
	//update the board based on current play. 
	vector<Point*> points = b.getPlayedPoints();
	for (int i = 0; i < points.size(); i++) {
		int x = points.at(i)->getX();
		int y = points.at(i)->getY();

		PlayerType player = b.readBoard(*points.at(i));
		board[x][y] = player;
	}

	update();
}

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

	painter.setPen(QPen(QColor(51, 102, 204), 2.0, Qt::SolidLine, Qt::RoundCap));
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

void BoardWidget::resetBoard() {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			board[i][j] = 0;
		}
	}
	update();
}

QPainterPath BoardWidget::getXPath(int row, int col) {
	QPainterPath *path = new QPainterPath();
	int thirdHeight = height() / 3;
	int thirdWidth = width() / 3;

	//define a bounding rect. 
	QRect rect = *rects[row][col];

	path->moveTo(rect.left(), rect.top());
	path->lineTo(rect.right(), rect.bottom());
	path->moveTo(rect.right(), rect.top());
	path->lineTo(rect.left(), rect.bottom());

	return *path;
}

QPainterPath BoardWidget::getOPath(int row, int col) {
	QPainterPath *path = new QPainterPath();
	int thirdHeight = height() / 3;
	int thirdWidth = width() / 3;

	//define a bounding rect. 
	QRect rect = *rects[row][col];

	qreal radius = qMin(rect.width()/2, rect.height()/2);
	path->addEllipse(rect.center(), radius, radius);
	return *path;
}

QSize BoardWidget::sizeHint() const {
	return QSize(100, 100);
}

void BoardWidget::mousePressEvent(QMouseEvent *event) {
	int x = event->x();
	int y = event->y();

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (rects[i][j]->contains(x, y)) {
				emit userPlayed(Point(i, j));
				break;
			}
		}
	}
}

Board::Board() {
	availablePoints = new vector<Point*>();
	playedPoints = new vector<Point*>();

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			board[i][j] = NO_ONE;
			availablePoints->push_back(new Point(i, j));
		}
	}
}

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

bool Board::isGameOver() {
	return (hasXWon() || hasOWon() || availablePoints->size() == 0);
}

bool Board::hasXWon() {
	if ((board[0][0] == board[1][1] && board[0][0] == board[2][2] && board[0][0] == USER) 
		|| (board[0][2] == board[1][1] && board[0][2] == board[2][0] && board[0][2] == USER)) {
		// System.out.println("O Diagonal Win");
		return true;
	}
	for (int i = 0; i < 3; ++i) {
		if ((board[i][0] == board[i][1] && board[i][0] == board[i][2] && board[i][0] == USER)
			|| (board[0][i] == board[1][i] && board[0][i] == board[2][i] && board[0][i] == USER)) {
			//  System.out.println("O Row or Column win");
			return true;
		}
	}

	return false;
}

bool Board::hasOWon() {
	if ((board[0][0] == board[1][1] && board[0][0] == board[2][2] && board[0][0] == COMPUTER)
		|| (board[0][2] == board[1][1] && board[0][2] == board[2][0] && board[0][2] == COMPUTER)) {
		// System.out.println("O Diagonal Win");
		return true;
	}
	for (int i = 0; i < 3; ++i) {
		if ((board[i][0] == board[i][1] && board[i][0] == board[i][2] && board[i][0] == COMPUTER)
			|| (board[0][i] == board[1][i] && board[0][i] == board[2][i] && board[0][i] == COMPUTER)) {
			//  System.out.println("O Row or Column win");
			return true;
		}
	}

	return false;
}

vector<Point*> Board::getPlayedPoints() {
	return *playedPoints;
}

vector<Point*> Board::getAvailablePoints() {
	return *availablePoints;
}

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

void Board::clearBoard() {
	availablePoints->clear();
	availablePoints = new vector<Point*>();
	playedPoints->clear();
	playedPoints = new vector<Point*>();
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			board[i][j] = NO_ONE;
			availablePoints->push_back(new Point(i, j));
		}
	}
}

TicTacToe::TicTacToe(Board *board) {
	mBoard = board;
}

Board TicTacToe::getBoard() {
	return *mBoard;
}

ComputerPlayer::ComputerPlayer(PlayerType type) : Player(type) {
	depth = 0;
}

void ComputerPlayer::performMove(Board &board) {
	depth = 0;
	ComputerMove move = getBestMove(board, new Player(COMPUTER));
	board.addAPlay(move.p, new Player(COMPUTER));
}

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

	vector<Point*> availablePoints = board.getAvailablePoints();
	vector<ComputerMove> moves;

	for (int i = 0; i < availablePoints.size(); i++) {
		ComputerMove move;
		move.p = *availablePoints.at(i);
		
		board.addAPlay(*availablePoints.at(i), player);
	
		if (player->getPlayerType() == USER) {
			move.score = getBestMove(board, new Player(COMPUTER)).score;
		}
		else if (player->getPlayerType() == COMPUTER) {
			move.score = getBestMove(board, new Player(USER)).score;
		}
		moves.push_back(move);
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

	return moves[bestMove];
}