#pragma once

#include "tictactoe/detail/board.h"

namespace tictactoe
{
	class computer_player : public player
	{
	public:
		enum class difficulty
		{
			easy,
			medium,
			hard,
			impossible
		};

		computer_player(const difficulty& diff = difficulty::easy,
			const player_shape& shape = player_shape::circle);
		void perform_move(const std::weak_ptr<board>& board);
		void set_difficulty(const difficulty& diff);
	private:
		unsigned int max_depth_ = 1000;
		int score_board(tictactoe::board& board) const;
		int minimax(tictactoe::board board, int depth, bool is_maximizer, int max_depth);
		player_move get_best_move(tictactoe::board& board, const player& current_player);
	};

	inline bool operator<(const player_move& left, const player_move& right)
	{
		return left.score < right.score;
	}

	inline bool operator==(const player_move& left, const player_move& right)
	{
		return left.score == right.score;
	}
}