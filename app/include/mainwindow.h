#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>

#include "boardwidget.h"

namespace Ui {
class MainWindow;
}

enum game_state {waiting, playing, done};

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
	void onUserPlayed(QPoint p);

private:
    Ui::MainWindow *ui;
	tictactoe::board board_;
	std::unique_ptr<BoardWidget> board_widget_;
	game_state game_state_;
	tictactoe::player user_{ tictactoe::player_shape::cross };
	tictactoe::computer_player computer_player_;
	tictactoe::player current_player_{};
};

#endif // MAINWINDOW_H
