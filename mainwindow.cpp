#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	ui->spinBox->setMaximum(10);
	ui->spinBox->setMinimum(1);
	connect(ui->startButton, SIGNAL(clicked()), this, SLOT(onStartClicked()));
	board = new Board();
	game = new TicTacToe(board);
	ui->verticalLayout->addWidget(board);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onStartClicked() {

}