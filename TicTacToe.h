#ifndef TICTACTOE_H
#define TICTACTOE_H

#include "qlist.h"
#include "qstring.h"
#include "qwidget.h"
#include "qpainter.h"
#include "qevent.h"
#include "qcolor.h"

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

class Board : public QWidget {
	Q_OBJECT
public:
	Board();
	Board(QWidget *parent);
	bool hasXWon();
	bool hasOWon();
	bool isGameOver();
	QList<Point*> getAvailablePoints();
	void addAPlay(Point play, Player player);
	QList<Point*> *availablePoints = new QList<Point*>();

private:
	int board[3][3];

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