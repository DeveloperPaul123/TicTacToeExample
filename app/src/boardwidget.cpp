#include "boardwidget.h"

#include <QWidget>
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <utility>

/**
* Widget to display a tic tac toe board.
* @param parent the QWidget parent of this widget.
*/
BoardWidget::BoardWidget(std::shared_ptr<tictactoe::board> board, QWidget* parent)
    : QWidget(parent)
{
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    resize(BoardWidget::sizeHint());
    const auto third_width = width() / 3;
    const auto third_height = height() / 3;
    for (auto i = 0; i < 3; i++) {
        for (auto j = 0; j < 3; j++) {
            rects_[i][j] = QRect(j * third_width, i * third_height, third_width, third_height);
        }
    }
    board_ptr_ = std::move(board);
    board_ptr_->add_callback([this] { update(); });
}

/**
* Called when the widget draws itself.
* @param event the QPaintEvent passed to this widget.
*/
void BoardWidget::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.setBrush(Qt::NoBrush);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(QColor(0, 0, 0), 2.0, Qt::SolidLine, Qt::RoundCap, Qt::BevelJoin));

    const auto third_width = width() / 3;
    const auto third_height = height() / 3;

    //draw the actual board. 
    painter.drawLine(third_width, 0.0, third_width, height());
    painter.drawLine(2 * third_width, 0.0, 2 * third_width, height());
    painter.drawLine(0.0, third_height, width(), third_height);
    painter.drawLine(0.0, 2 * third_height, width(), 2 * third_height);

    for (auto i = 0; i < 3; i++) {
        for (auto j = 0; j < 3; j++) {
            rects_[i][j] = QRect(j * third_width + 3, i * third_height + 3,
                third_width - 3, third_height - 3);
        }
    }

    painter.setPen(QPen(QColor(51, 102, 204), 6.0, Qt::SolidLine, Qt::RoundCap));
    //draw x's and o's
    for (auto i = 0; i < 3; i++)
    {
        for (auto j = 0; j < 3; j++)
        {
            //user is X.
            auto path = getShapePath(i, j, board_ptr_->read_board(i, j));
            painter.drawPath(path);
        }
    }

}

QPainterPath BoardWidget::getShapePath(int row, int col, const tictactoe::player_shape& shape) const
{
    QPainterPath path;

    constexpr auto padding = 15;
    //define a bounding rect. 
    const auto rect = rects_[row][col];

    if (shape == tictactoe::player_shape::cross)
    {
        path.moveTo(rect.left() + padding, rect.top() + padding);
        path.lineTo(rect.right() - padding, rect.bottom() - padding);
        path.moveTo(rect.right() - padding, rect.top() + padding);
        path.lineTo(rect.left() + padding, rect.bottom() - padding);
    }
    else if (shape == tictactoe::player_shape::circle)
    {
        qreal radius = qMin(rect.width() / 2, rect.height() / 2);
        radius -= padding;
        path.addEllipse(rect.center(), radius, radius);
    }

    return path;
}

/**
* Size hint for the widget. Used when being laid out.
* @return QSize the size we want for the widget.
*/
QSize BoardWidget::sizeHint() const
{
    return {100, 100};
}

/**
* Called when the user clicks the board.
* @param event the QMouseEvent passed to the widget.
*/
void BoardWidget::mousePressEvent(QMouseEvent* event) {
    const auto x = event->x();
    const auto y = event->y();
    //check where they clicked (i.e. which square on the board.
    for (auto i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (rects_[i][j].contains(x, y)) {
                emit userPlayed(QPoint(i, j));
                break;
            }
        }
    }
}