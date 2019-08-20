#ifndef TICTACTOE_H
#define TICTACTOE_H

#include <QList>
#include <QString>
#include <QWidget>
#include <QPainter>
#include <QEvent>
#include <QColor>
#include <QPainterPath>
#include <QRect>
#include <QtMath>

#include <vector>
#include <stdlib.h>

class Point {
public:
	Point();
	Point(int x, int y);
	QString toString();
	int getX();
	int getY();
	bool equals(Point p);

private:
	int mX = 0, mY = 0;
};

enum PlayerType { NO_ONE=0, USER = 1, COMPUTER = 2 };

class Player {
public:
	Player();
	Player(PlayerType type);
	PlayerType getPlayerType();
	void setPlayerType(PlayerType type);

private:
	PlayerType mType;
};

class Board {
public:
	Board();
	bool hasXWon();
	bool hasOWon();
	bool isGameOver();
	std::vector<Point*> getAvailablePoints();
	std::vector<Point*> getPlayedPoints();
	bool addAPlay(Point play, Player *player);
	void removePlay(Point play);
	PlayerType readBoard(Point p);
	void clearBoard();

private:
	int board[3][3];
	std::vector<Point*> *availablePoints;
	std::vector<Point*> *playedPoints;
};

class BoardWidget : public QWidget {
	Q_OBJECT;
public:
	BoardWidget(QWidget*parent);
	void updateBoard(Board &board);
	void resetBoard();
private:
	int board[3][3];
	QRect* rects[3][3];
	QPainterPath getXPath(int row, int col);
	QPainterPath getOPath(int row, int col);

signals:
	void clicked();
	void userPlayed(Point p);

protected:
	void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
	QSize sizeHint() const;
	void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
};

struct ComputerMove {
	ComputerMove() {};
	ComputerMove(int s){ score = s; };
	Point p;
	int score;
};

class ComputerPlayer : public Player {
public:
	ComputerPlayer(PlayerType type = COMPUTER);
	void performMove(Board &board);
	void setDifficulty(int diff);
private:
	int depth, maxDepth = 1000;
	int mScore, mOppScore;
	ComputerMove getBestMove(Board& board, Player *player);
};

#endif