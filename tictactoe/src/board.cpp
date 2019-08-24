#include "tictactoe/detail/board.h"

namespace tictactoe
{
	board::board(const size_type &size)
		:size_(size)
	{
		board_ = std::make_unique<int[]>(size * size);
		clear();
	}

	bool board::has_player_won(const player_shape& shape) const
	{
		if ((read_board(0, 0) == read_board(1, 1) && 
				read_board(0, 0) == read_board(2, 2) && read_board(0, 0) == shape)
			|| (read_board(0, 2) == read_board(1, 1) && 
				read_board(0, 2) == read_board(2,0) && read_board(0, 2) == shape)) 
		{
			return true;
		}

		// check vertical and horizontal
		for (int i = 0; i < 3; ++i) 
		{
			if ((read_board(i, 0) == read_board(i,1) && 
				read_board(i,0) == read_board(i, 2) && read_board(i, 0) == shape)
				|| (read_board(0, i) == read_board(1, i) && 
					read_board(0, i) == read_board(2, i) && read_board(0, i) == shape)) 
			{
				return true;
			}
		}

		return false;
	}

	bool board::is_game_over() const
	{
		const auto available_points = get_available_positions();
		return (has_player_won(player_shape::circle) || has_player_won(player_shape::cross)) || available_points.empty();
	}

	std::vector<point> board::get_available_positions() const
	{
		std::vector<point> available_points;
		for (auto row = 0; row < size_; row++)
		{
			for (auto col = 0; col < size_; col++)
			{
				if (read_board(row, col) == player_shape::open)
				{
					// not that x is the column, y is row.
					available_points.emplace_back(point(col, row ));
				}
			}
		}
		return available_points;
	}

	std::vector<point> board::get_played_positions() const
	{
		std::vector<point> played_points;
		for (auto row = 0; row < size_; row++)
		{
			for (auto col = 0; col < size_; col++)
			{
				if (read_board(row, col) == player_shape::open)
				{
					// not that x is the column, y is row.
					played_points.emplace_back(point(col, row));
				}
			}
		}

		return played_points;
	}

	bool board::add_play(const point& play, const player_shape& player)
	{
		auto available_points = get_available_positions();
		const auto available_point = std::find(available_points.begin(), available_points.end(), play);
		if(available_point != std::end(available_points))
		{
			write_board(play, player);
			return true;
		}

		return false;
	}

	bool board::remove_play(const point& play)
	{
		auto played_points = get_played_positions();
		const auto played_point = std::find(played_points.begin(), played_points.end(), play);
		if(played_point != std::end(played_points))
		{
			write_board(play, player_shape::open);
			return true;
		}

		return false;
	}

	void board::clear()
	{
		std::fill(board_.get(),
			board_.get() + (size_ * size_),
			static_cast<int>(player_shape::open));

		for (auto callback : callbacks_)
		{
			if (callback)
			{
				callback->board_changed();
			}
		}
	}

	player_shape board::read_board(int row, int col) const
	{
		const auto index = col + row * size_;
		return player_shape(board_[index]);
	}

	void board::add_callback(board_callback* callback)
	{
		callbacks_.emplace_back(callback);
	}

	board::size_type board::size() const
	{
		return size_;
	}

	player_shape board::read_board(const point& pt) const
	{
		// now that y is the row, x is the column
		return read_board(pt.y(), pt.x());
	}

	void board::write_board(const point& pt, const player_shape& shape) const
	{
		const auto index = pt.x() + pt.y() * size_;
		board_[index] = static_cast<int>(shape);
		for(auto callback : callbacks_)
		{
			if(callback)
			{
				callback->board_changed();
			}
		}
	}
}
