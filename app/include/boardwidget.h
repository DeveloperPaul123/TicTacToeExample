#pragma once

#include <QWidget>
#include <QPoint>
#include <tictactoe/tictactoe>

class BoardWidget : public QWidget , tictactoe::board::board_callback 
{
	Q_OBJECT;
public:
	BoardWidget(std::shared_ptr<tictactoe::board> board, QWidget* parent = Q_NULLPTR);
	void board_changed() override;

private:
	std::shared_ptr<tictactoe::board> board_ptr_;
	QRect rects_[3][3];

	QPainterPath getShapePath(int row, int col, const tictactoe::player_shape& shape);

signals:
	void clicked();
	void userPlayed(QPoint p);

protected:
	void paintEvent(QPaintEvent* event) Q_DECL_OVERRIDE;
	QSize sizeHint() const override;
	void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
};
