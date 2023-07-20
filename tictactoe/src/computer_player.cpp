#include "tictactoe/detail/computer_player.h"
#include <algorithm>
#include <cassert>

namespace tictactoe
{
	computer_player::computer_player(const difficulty& diff, const player_shape& shape) : player()
	{
		this->shape = shape;
		set_difficulty(diff);
	}

	void computer_player::perform_move(const std::weak_ptr<board>& board)
	{
		if (const auto brd_ptr = board.lock())
		{
			if (brd_ptr->is_game_over()) return;
			const auto computer_move = get_best_move(*brd_ptr, *this);
			assert(brd_ptr->add_play(computer_move.play, this->shape));
		}

	}

	void computer_player::set_difficulty(const difficulty& diff)
	{
		switch (diff) {
		case difficulty::easy:
			max_depth_ = 1;
			break;
		case difficulty::medium:
			max_depth_ = 2;
			break;
		case difficulty::hard:
			max_depth_ = 5;
			break;
		case difficulty::impossible:
			max_depth_ = 8;
			break;
		}
	}

	int computer_player::score_board(tictactoe::board& board) const
	{
		if (board.has_player_won(this->shape))
		{
			return 10;
		}
		if (board.has_player_won(player_shape::cross))
		{
			return -10;
		}
		return 0;
	}

	int computer_player::minimax(tictactoe::board board, int depth, bool is_maximizer, int max_depth)
	{
		const auto score = score_board(board);
		// if score is 0, no one has won yet
		if (score != 0 || depth >= max_depth)
		{
			return score;
		}

		if (board.get_available_positions().empty())
		{
			return 0;
		}

		if (is_maximizer)
		{
			int best = std::numeric_limits<int>::min();
			for (std::size_t row = 0; row < board.size(); row++)
			{
				for (std::size_t col = 0; col < board.size(); col++)
				{
					if (board.read_board(row, col) == player_shape::open)
					{
						if (board.add_play(point(row, col), this->shape))
						{
							best = std::max(best, minimax(board, depth + 1, !is_maximizer, max_depth));

						}
						assert(board.remove_play(tictactoe::point(row, col)));

					}
				}
			}
			return best;
		}

		// minimizer
		int best = std::numeric_limits<int>::max();
		for (std::size_t row = 0; row < board.size(); row++)
		{
			for (std::size_t col = 0; col < board.size(); col++)
			{
				if (board.read_board(row, col) == player_shape::open)
				{
					assert(board.add_play(tictactoe::point(row, col), player_shape::cross));
					best = std::min(best, minimax(board, depth + 1, !is_maximizer, max_depth));
					assert(board.remove_play(tictactoe::point(row, col)));
				}
			}
		}
		return best;
	}

	player_move computer_player::get_best_move(tictactoe::board& board, const player& current_player)
	{
		int best_value = std::numeric_limits<int>::min();
		tictactoe::point best_move(-1, -1);

		//available points. 
		const auto available_points = board.get_available_positions();

		// nothing we can do, return default move
		if (available_points.empty()) return {};

		//go through moves
		for (auto available_point : available_points)
		{
			auto current_play = available_point;
			//add player_move. 
			assert(board.add_play(current_play, current_player.shape));
			const auto move_value = minimax(board, 0, false, max_depth_);

			//remove the player_move from the board. 
			assert(board.remove_play(available_point));

			if (move_value > best_value)
			{
				best_value = move_value;
				best_move = current_play;
			}
		}

		player_move move;
		move.play = best_move;
		move.score = best_value;
		return move;
	}

}
