#ifndef TICTACTOE_H
#define TICTACTOE_H

#include "qlist.h"
#include "qstring.h"
#include "qwidget.h"
#include "qpainter.h"
#include "qevent.h"
#include "qcolor.h"
#include <vector>

using namespace std;

class Point {
public:
	Point(int x, int y);
	QString toString();
	int getX();
	int getY();
	bool equals(Point p);

private:
	int mX = 0, mY = 0;
};

enum PlayerType {USER=1, COMPUTER};

class Player {
public:
	Player(PlayerType type);
	PlayerType getPlayerType();
private:
	PlayerType mType;
};

class Board {
public:
	Board();
	bool hasXWon();
	bool hasOWon();
	bool isGameOver();
	vector<Point*> getAvailablePoints();
	void addAPlay(Point play, Player player);

private:
	int board[3][3];
	vector<Point*> availablePoints;
};

class BoardWidget : public QWidget {
public:
	BoardWidget(QWidget*parent);
	void updateBoard(const Board &board);

protected:
	void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
	QSize sizeHint() const;
};

class TicTacToe {
public:
	TicTacToe(Board *board);
	Board getBoard();
private:
	Board *mBoard;
};

#endif