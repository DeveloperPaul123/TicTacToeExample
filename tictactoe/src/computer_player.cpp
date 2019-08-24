#include "tictactoe/detail/computer_player.h"
#include <algorithm>

namespace tictactoe
{
	computer_player::computer_player(const difficulty& diff, const player_shape& shape) : player()
	{
		this->shape = shape;
		set_difficulty(diff);
	}

	void computer_player::perform_move(std::weak_ptr<board> board)
	{
		depth_ = 0;
		if(auto brd_ptr = board.lock())
		{
			const auto computer_move = get_best_move(*brd_ptr, *this);
			brd_ptr->add_play(computer_move.play, this->shape);
		}
		
	}

	void computer_player::set_difficulty(const difficulty& diff)
	{
		int random = 0;
		switch (diff) {
		case difficulty::easy:
			max_depth_ = 1;
			random = rand() % 90 + 87;
			score_ = 100 + random;
			opponent_score_ = 100 - random;
			break;
		case difficulty::medium:
			max_depth_ = 2;
			random = rand() % 80 + 70;
			score_ = 100 + random;
			opponent_score_ = 100 - random;
			break;
		case difficulty::hard:
			max_depth_ = 5;
			random = rand() % 50 + 20;
			score_ = 100 + random;
			opponent_score_ = 100 - random;
			break;
		case difficulty::impossible:
			max_depth_ = 8;
			score_ = 100;
			opponent_score_ = 100;
			break;
		}
	}

	int computer_player::score_board(tictactoe::board& board)
	{
		if (board.has_player_won(this->shape))
		{
			return 10;
		}
		else if (board.has_player_won(player_shape::cross))
		{
			return -10;
		}
		return 0;
	}

	int computer_player::minimax(tictactoe::board board, int depth, bool is_maximizer)
	{
		const auto score = score_board(board);
		if (score != 0)
		{
			return score;
		}

		if (board.get_available_positions().empty())
		{
			return 0;
		}

		if (is_maximizer)
		{
			int best = -1000;
			for (auto row = 0; row < board.size(); row++)
			{
				for (auto col = 0; col < board.size(); col++)
				{
					if (board.read_board(row, col) == player_shape::open)
					{
						board.add_play(tictactoe::point(col, row), this->shape);
						best = std::max(best, minimax(board, depth + 1, !is_maximizer));
						board.remove_play(tictactoe::point(col, row));
					}
				}
			}
			return best;
		}
		else
		{
			int best = 1000;
			for (auto row = 0; row < board.size(); row++)
			{
				for (auto col = 0; col < board.size(); col++)
				{
					if (board.read_board(row, col) == player_shape::open)
					{
						board.add_play(tictactoe::point(col, row), player_shape::cross);
						best = std::min(best, minimax(board, depth + 1, !is_maximizer));
						board.remove_play(tictactoe::point(col, row));
					}
				}
			}
			return best;
		}
	}

	player_move computer_player::get_best_move(tictactoe::board &board, const player& current_player)
	{
		int best_value = -1000;
		tictactoe::point best_move(-1, -1);
	
		//available points. 
		auto available_points = board.get_available_positions();

		//go through moves. 
		for (std::vector<player_move>::size_type i = 0; i < available_points.size(); i++) {
			auto current_play = available_points.at(i);
			//add player_move. 
			board.add_play(current_play, current_player.shape);
			auto move_value = minimax(board, 0, false);

			//remove the player_move from the board. 
			board.remove_play(available_points.at(i));

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
