#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "TicTacToe.h"
#include "qmessagebox.h"

namespace Ui {
class MainWindow;
}

enum GameState {WAITING, PLAYING, DONE};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

	void startGame();
	bool checkForWin();

public slots:
	void onStartClicked();
	void onUserPlayed(Point p);

private:
    Ui::MainWindow *ui;
	Board *board;
	BoardWidget *boardWidget;
	GameState mState;
	Player *user;
	ComputerPlayer *compPlayer;
	Player currentPlayer;
};

#endif // MAINWINDOW_H
