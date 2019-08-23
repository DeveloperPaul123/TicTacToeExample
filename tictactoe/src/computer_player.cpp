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
		const auto computer_move = get_best_move(board, *this);
		if(auto brd_ptr = board.lock())
		{
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

	player_move computer_player::get_best_move(std::weak_ptr<board> board_ptr, const player& current_player)
	{

		if(auto board = board_ptr.lock())
		{
			//base case, check for end state. 
			if (board->has_player_won(player_shape::circle)) {
				//computer won. User is always x.
				return player_move(score_);
			}
			if (board->has_player_won(player_shape::cross)) {
				//computer lost, he doesn't like this. 
				return player_move(-1 * opponent_score_);
			}
			if (board->is_game_over()) {
				//tie.
				return player_move(0);
			}

			//available points. 
			auto available_points = board->get_available_positions();
			//vector to hold all the moves we make to look through them later. 
			std::vector<player_move> moves;

			//go through moves. 
			for (std::vector<player_move>::size_type i = 0; i < available_points.size(); i++) {
				player_move current_move;
				auto current_play = available_points.at(i);
				current_move.play = current_play;
				//add player_move. 
				board->add_play(current_play, current_player.shape);
				//recursive call. 
				if (current_player.shape == player_shape::cross) {
					current_move.score = get_best_move(board, player{ player_shape::circle }).score;
				}
				else if (current_player.shape == player_shape::circle) {
					current_move.score = get_best_move(board, player{ player_shape::cross }).score;
				}
				//remember the player_move we made. 
				moves.emplace_back(current_move);

				//remove the player_move from the board. 
				board->remove_play(available_points.at(i));
			}


			// Pick best player_move for the current player.
			std::sort(moves.begin(), moves.end());

			if (current_player.shape == player_shape::circle)
			{
				// computer won	so we want the highest value
				return *(moves.end() - 1);
			}

			return *moves.begin();
		}

		// default return
		return player_move(0);
	}
}
