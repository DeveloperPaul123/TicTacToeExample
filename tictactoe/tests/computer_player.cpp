#include <snitch/snitch.hpp>

#include <tictactoe/detail/board.h>
#include <tictactoe/detail/computer_player.h>

using namespace tictactoe;

TEST_CASE("Check that computer favors center at beginning of game")
{

	board board{};
	computer_player computer(computer_player::difficulty::impossible);

	computer.perform_move(board);
	const auto& played_moves = board.get_played_positions();
	CHECK(played_moves.size() == 1);
	CHECK(played_moves[0] == point{ 1, 1 });
}

TEST_CASE("Check that computer favors winning over blocking")
{
	auto x = player_shape::cross;
	auto o = player_shape::circle;
	auto n = player_shape::open;

	// computer is 'o' for all of these states
	const std::vector<std::initializer_list<player_shape>> board_situations {
		{
			o, o, n,
			n, x, n,
			n, x, n
		},
		{
			o, n, n,
			o, x, n,
			n, x, n
		},
		{
			x, x, o,
			n, o, n,
			n, x, n
		},
		{
			x, n, x,
			o, o, n,
			n, n, n
		}
	};

	for (const auto& board_state : board_situations)
	{
		board board{ board_state };
		computer_player computer(computer_player::difficulty::impossible, o);

		computer.perform_move(board);
		CHECK(board.is_game_over());
		CHECK(board.has_player_won(o));
	}
}

TEST_CASE("Check that computer blocks player if it can't win")
{
	auto x = player_shape::cross;
	auto o = player_shape::circle;
	auto n = player_shape::open;

	// computer is 'o' for all of these states
	const std::vector<std::initializer_list<player_shape>> board_situations {
		{
			o, o, x,
			n, n, x,
			n, n, n
		},
		{
			o, n, x,
			n, n, x,
			n, n, n
		},
		{
			x, x, n,
			n, o, n,
			n, n, n
		},
		{
			x, n, o,
			o, x, n,
			n, n, n
		},
		{
			n, n, o,
			n, x, x,
			n, n, n
		},
		{
			o, n, n,
			x, x, n,
			n, n, n
		}
	};

	for (const auto& board_state : board_situations)
	{
		board board{ board_state };
		computer_player computer(computer_player::difficulty::impossible, o);

		computer.perform_move(board);
		CHECK_FALSE(board.is_game_over());
		CHECK(board.was_player_blocked(x));
	}
}
