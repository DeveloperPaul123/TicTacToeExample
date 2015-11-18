#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "TicTacToe.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
	void onStartClicked();
private:
    Ui::MainWindow *ui;
	TicTacToe *game;
	Board *board;
	
};

#endif // MAINWINDOW_H
