#include "TicTacToe.h"

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

Player::Player(PlayerType type) {
	type = mType;
}

PlayerType Player::getPlayerType() {
	return mType;
}

BoardWidget::BoardWidget(QWidget* parent) : QWidget(parent) {
	setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
	resize(sizeHint());
}

void BoardWidget::updateBoard(const Board &board) {
	//update the board based on current play. 
}

void BoardWidget::paintEvent(QPaintEvent *event) {
	QPainter painter(this);
	painter.setBrush(Qt::NoBrush);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(QPen(QColor(0, 0, 0), 2.0, Qt::SolidLine, Qt::RoundCap, Qt::BevelJoin));

	int thirdWidth = width() / 3;
	int thirdHeight = height() / 3;

	painter.drawLine(0.0, 0.0, width(), height());
}

QSize BoardWidget::sizeHint() const {
	return QSize(100, 100);
}

Board::Board() {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; i < 3; i++) {
			board[i][j] = 0;
			availablePoints.push_back(new Point(i, j));
		}
	}

}

bool Board::isGameOver() {
	return (hasXWon() || hasOWon() || availablePoints.size() == 0);
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

vector<Point*> Board::getAvailablePoints() {
	return availablePoints;
}

void Board::addAPlay(Point play, Player player) {
	for (int i = 0; i < availablePoints.size(); i++) {
		Point *p = availablePoints.at(i);
		if (p->equals(play)){
			availablePoints.erase(availablePoints.begin() + i);
			board[p->getX()][p->getY()] = player.getPlayerType();
			break;
		}
	}
}


TicTacToe::TicTacToe(Board *board) {
	mBoard = board;
}

Board TicTacToe::getBoard() {
	return *mBoard;
}