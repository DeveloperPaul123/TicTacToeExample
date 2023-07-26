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
	const auto board_state = {
		o, o, n,
		n, x, n,
		n, x, n
	};

	board board{ board_state };
	computer_player computer(computer_player::difficulty::impossible, o);

	computer.perform_move(board);
	CHECK(board.is_game_over());
	CHECK(board.has_player_won(o));
}
