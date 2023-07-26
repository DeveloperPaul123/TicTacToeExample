#pragma once

#include "tictactoe/detail/board.h"
#include <optional>
#include <stdexcept>

namespace tictactoe
{
	enum class player_shape
	{
		open = 0,
		circle,
		cross
	};

	static player_shape get_opposite_shape(const player_shape& shape)
	{
		if (shape == player_shape::circle)
		{
			return player_shape::cross;
		}

		if (shape == player_shape::cross)
		{
			return player_shape::circle;
		}

		throw std::runtime_error("Invalid shape");
	}

	struct player_move
	{
		player_move() = default;
		explicit player_move(std::size_t scr)
		{
			score = scr;
			play = point(0, 0);
		}
		point play{};
		std::size_t score{ 0 };
	};

	struct player
	{
		player_shape shape;
	};
}
