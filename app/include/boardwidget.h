#pragma once

#include <QWidget>
#include <QPoint>
#include <tictactoe/tictactoe>

class BoardWidget : public QWidget
{
	Q_OBJECT

public:
	BoardWidget(tictactoe::board* board, QWidget* parent = Q_NULLPTR);
	~BoardWidget() override;
private:
	tictactoe::board* board_ptr_;
	QRect rects_[3][3];

	[[nodiscard]] QPainterPath getShapePath(int row, int col, const tictactoe::player_shape& shape) const;

signals:
	void clicked();
	void userPlayed(QPoint p);

protected:
	void paintEvent(QPaintEvent* event) Q_DECL_OVERRIDE;
	[[nodiscard]] QSize sizeHint() const override;
	void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
};
