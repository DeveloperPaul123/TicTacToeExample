#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	board_(3)
{
	ui->setupUi(this);
	connect(ui->startButton, &QPushButton::clicked, this, &MainWindow::onStartClicked);
	board_widget_ = std::make_unique<BoardWidget>(&board_, this);
	ui->verticalLayout->addWidget(board_widget_.get());
	game_state_ = waiting;

	//user goes first
	connect(board_widget_.get(), &BoardWidget::userPlayed,
		this, &MainWindow::onUserPlayed);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::startGame()
{
	game_state_ = playing;
	if (ui->checkBox->isChecked()) {
		computer_player_.perform_move(board_);
	}
}

void MainWindow::onStartClicked() {
	int difficulty_index = ui->comboBox->currentIndex();
	computer_player_.set_difficulty(static_cast<tictactoe::computer_player::difficulty>(difficulty_index));
	if (game_state_ == waiting) {
		startGame();
		return;
	}
	if (game_state_ == playing) {
		//clear everything. 
		board_.clear();
		game_state_ = playing;
	}
	else if (game_state_ == done) {
		//clear everything. 
		board_.clear();
		game_state_ = playing;
	}

	if (ui->checkBox->isChecked()) {
		computer_player_.perform_move(board_);
	}
}

void MainWindow::onUserPlayed(QPoint p) {

	if (game_state_ == playing) {
		if (checkForWin())
		{
			// ?
		}
		else
		{
			if (board_.add_play(tictactoe::point(p.x(), p.y()), user_.shape))
			{
				computer_player_.perform_move(board_);
			}
			checkForWin();
		}
	}
}

bool MainWindow::checkForWin()
{
	auto show_box = false;
	QString message;
	if (board_.has_player_won(tictactoe::player_shape::circle))
	{
		message = "You lost!";
		game_state_ = done;
		show_box = true;
	}
	else if (board_.has_player_won(tictactoe::player_shape::cross))
	{
		message = "You won!";
		game_state_ = done;
		show_box = true;
	}
	if (board_.is_game_over())
	{
		message = "Draw!";
		game_state_ = done;
		show_box = true;
	}

	if (show_box)
	{
		QMessageBox::information(this, tr("Game Over"), message);
	}

	return show_box;
}
