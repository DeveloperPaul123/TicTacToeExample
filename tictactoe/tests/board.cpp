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

using namespace tictactoe;

TEST_CASE("Check default state of board")
{
    const board board;
    CHECK(board.grid_size() == 9);
}

TEST_CASE("Clearing board works")
{
    const board board;
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
    const board board{};
    constexpr auto player = player_shape::cross;
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
    board board{};

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

TEST_CASE("Check if player was blocked")
{
    auto x = player_shape::cross;
    auto o = player_shape::circle;
    auto n = player_shape::open;

    // computer is 'o' for all of these states
    const std::vector<std::initializer_list<player_shape>> board_situations {
        {
            o, o, x,
                n, n, x,
                n, n, o
        },
        {
            o, n, x,
            o, n, x,
            n, n, o
        },
        {
            x, x, o,
            n, o, n,
            n, n, n
        },
        {
            x, n, o,
            o, x, n,
            n, n, o
        },
        {
            o, o, x,
            n, n, x,
            n, n, o
        },
    };

    const std::vector<std::initializer_list<player_shape>> non_blocked_situations {
        {
            o, o, x,
            n, n, x,
            o, n, n
        },
    };

    for (const auto& board_state : board_situations)
    {
        board board{ board_state };
        CHECK(board.was_player_blocked(x));
    }

    for (const auto& board_state : non_blocked_situations) {
        board board{ board_state };
        CHECK_FALSE(board.was_player_blocked(x));
    }
}
