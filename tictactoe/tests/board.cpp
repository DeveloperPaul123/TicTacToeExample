#include <snitch/snitch.hpp>

#include <tictactoe/detail/board.h>
#include <atomic>
#include <vector>

const auto top_row_points = std::vector<tictactoe::point>{ {0,0}, {0,1}, {0,2} };
const auto middle_row_points = std::vector<tictactoe::point>{ {1,0}, {1,1}, {1,2} };
const auto bottom_row_points = std::vector<tictactoe::point>{ {2,0}, {2,1}, {2,2} };

const auto top_left_diagonal = std::vector<tictactoe::point>{ {0,0}, {1,1}, {2,2} };
const auto bottom_left_diagonal = std::vector<tictactoe::point>{ {0,2}, {1,1}, {2,0} };
const auto bottom_right_diagonal = std::vector<tictactoe::point>{ {2,2}, {1,1}, {0,0} };
const auto top_right_diagonal = std::vector<tictactoe::point>{ {2,0}, {1,1}, {0,2} };

const auto win_cases = std::vector{ top_row_points, middle_row_points, bottom_row_points, top_left_diagonal, top_right_diagonal, bottom_left_diagonal, bottom_right_diagonal };

TEST_CASE("Check default state of board")
{
	const tictactoe::board board;
	CHECK(board.grid_size() == 9);
}

TEST_CASE("Clearing board works")
{
	const tictactoe::board board;
	auto all_positions = board.get_available_positions();
	REQUIRE(all_positions.size() == board.grid_size());
	for (const auto& play_point : all_positions)
	{
		CHECK(board.add_play(play_point, tictactoe::player_shape::cross));
	}

	const auto& played_positions = board.get_played_positions();
	CHECK(played_positions.size() == board.grid_size());

	all_positions = board.get_available_positions();
	CHECK(all_positions.empty());

	board.clear();
	CHECK(board.get_played_positions().empty());
	CHECK_FALSE(board.get_available_positions().empty());
}

TEST_CASE("Check win detection")
{
	const tictactoe::board board{};
	constexpr auto player = tictactoe::player_shape::cross;
	for (const auto& win_positions : win_cases)
	{
		for (const auto& play_point : win_positions)
		{
			REQUIRE(board.add_play(play_point, player));
		}

		REQUIRE(board.is_game_over());

		// clear the board for the next test
		board.clear();
	}
}

TEST_CASE("Check callbacks are invoked properly")
{
	tictactoe::board board{};

	std::atomic_int64_t callback_count{ 0 };
	board.add_board_change_callback([&]
		{
			++callback_count;
		});

	board.add_board_change_callback([&]
		{
			++callback_count;
		});

	CHECK(board.add_play({ 0, 0 }, tictactoe::player_shape::cross));
	CHECK(callback_count == 2);
}
