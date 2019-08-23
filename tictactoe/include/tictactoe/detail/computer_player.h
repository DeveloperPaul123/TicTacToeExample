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

		computer_player(const difficulty &diff = difficulty::easy,
			const player_shape& shape = player_shape::circle);
		void perform_move(std::weak_ptr<board> board);
		void set_difficulty(const difficulty& diff);
	private:
		unsigned int depth_{0};
		unsigned int max_depth_ = 1000;
		int score_{0};
		int opponent_score_{0};

		player_move get_best_move(std::weak_ptr<board> board_ptr, const player& current_player);
	};

	inline bool operator<(const player_move &left, const player_move &right)
	{
		return left.score < right.score;
	}

	inline bool operator==(const player_move &left, const player_move &right)
	{
		return left.score == right.score;
	}
}