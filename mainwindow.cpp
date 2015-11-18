#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	connect(ui->startButton, SIGNAL(clicked()), this, SLOT(onStartClicked()));
	board = new Board();
	boardWidget = new BoardWidget(this);

	ui->verticalLayout->addWidget(boardWidget);
	mState = WAITING;
	//user goes first. 
	
	user = new Player(USER);
	compPlayer = new ComputerPlayer(COMPUTER);
	currentPlayer = *user;

	connect(boardWidget, SIGNAL(userPlayed(Point)), this, SLOT(onUserPlayed(Point)));
	
	//test case.
	/*board->addAPlay(Point(1, 2), user);
	board->addAPlay(Point(2, 2), user);
	board->addAPlay(Point(2, 0), compPlayer);
	board->addAPlay(Point(2, 1), compPlayer);

	boardWidget->updateBoard(*board);*/
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::startGame() {
	mState = PLAYING;
	if (ui->checkBox->isChecked()) {
		compPlayer->performMove(*board);
		boardWidget->updateBoard(*board);
	}
}

void MainWindow::onStartClicked() {
	int i = ui->comboBox->currentIndex();
	compPlayer->setDifficulty(i);
	if (mState == WAITING) {
		startGame();
		return;
	}
	else if (mState == PLAYING) {
		//clear everything. 
		board->clearBoard();
		boardWidget->resetBoard();
		mState = PLAYING;
	}
	else if (mState == DONE) {
		//clear everything. 
		board->clearBoard();
		boardWidget->resetBoard();
		mState = PLAYING;
	}

	if (ui->checkBox->isChecked()) {
		compPlayer->performMove(*board);
		boardWidget->updateBoard(*board);
	}
}

void MainWindow::onUserPlayed(Point p) {

	if (mState == PLAYING) {
		if (checkForWin()) {

		}
		else {
			if (board->addAPlay(p, user)) {
				boardWidget->updateBoard(Board(*board));
				compPlayer->performMove(*board);
				boardWidget->updateBoard(*board);
			}	
			checkForWin();
		}

	}
}

bool MainWindow::checkForWin() {
	if (board->hasOWon()) {
		QMessageBox *box = new QMessageBox();
		box->setWindowTitle("Game over!");
		box->setText("You lost!");
		box->show();
		mState = DONE;
		return true;
	}
	else if (board->hasXWon()) {
		QMessageBox *box = new QMessageBox();
		box->setWindowTitle("Game over!");
		box->setText("You won!");
		box->show();
		mState = DONE;
		return true;
	}
	else if (board->isGameOver()) {
		QMessageBox *box = new QMessageBox();
		box->setWindowTitle("Game over!");
		box->setText("Draw");
		box->show();
		mState = DONE;
		return true;
	}
	else{
		return false;
	}
}