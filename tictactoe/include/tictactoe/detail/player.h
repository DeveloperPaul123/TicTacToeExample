#pragma once

#include "tictactoe/detail/board.h"

namespace tictactoe
{
    enum class player_shape
    {
        open = 0,
        circle,
        cross
    };

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
