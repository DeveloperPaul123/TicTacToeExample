#include "tictactoe/detail/computer_player.h"
#include <algorithm>
#include <cassert>

namespace tictactoe
{
    computer_player::computer_player(const difficulty& diff, const player_shape& shape) : player()
    {
        this->shape = shape;
        set_difficulty(diff);
    }

    void computer_player::perform_move(board& board)
    {
        const auto computer_move = get_best_move(board, *this);
        assert(board.add_play(computer_move.play, this->shape));
    }

    void computer_player::set_difficulty(const difficulty& diff)
    {
        switch (diff) {
        case difficulty::easy:
            max_depth_ = 1;
            break;
        case difficulty::medium:
            max_depth_ = 2;
            break;
        case difficulty::hard:
            max_depth_ = 5;
            break;
        case difficulty::impossible:
            max_depth_ = 100;
            break;
        }
    }

    computer_player::score_type computer_player::score_board(const tictactoe::board& board, int depth) const
    {
        constexpr auto max_score = 1000;
        // check if we won
        if (board.has_player_won(shape))
        {
            return max_score / depth;
        }

        // check if opponent won
        if (board.has_player_won(get_opposite_shape(shape)))
        {
            return -1 * max_score / depth;
        }

        // todo handle even/odd board sizes (i.e. non 3x3)
        // favor center at begining of game
        if (board.get_played_positions().size() == 1 && board.read_board(1, 1) == shape)
        {
            // return the max score because we have already checked if we have won
            return max_score;
        }

        // check if we blocked the other player from winning
        if (board.was_player_blocked(get_opposite_shape(shape)))
        {
            return max_score / (depth + 2);
        }

        if (board.was_player_blocked(shape))
        {
            // we were blocked
            return -1 * max_score / (depth + 2);
        }

        return 0;
    }

    computer_player::score_type computer_player::negamax(tictactoe::board board, int depth, score_type alpha, score_type beta, player_shape shape)
    {
        const auto score = score_board(board, depth);
        // if score is 0, no one has won yet
        if (score != 0 || depth >= max_depth_)
        {
            return score;
        }

        if (board.get_available_positions().empty())
        {
            return 0;
        }

        auto best = std::numeric_limits<score_type>::min();
        const auto& available_positions = board.get_available_positions();

        for (auto& position : available_positions)
        {

            if (board.add_play(position, shape))
            {
                best = std::max(best, -negamax(board, depth + 1, -beta, -alpha, get_opposite_shape(shape)));
                assert(board.remove_play(position));
                alpha = std::max(alpha, best);
                if (alpha >= beta)
                {
                    return best;
                }
            }
        }
        return best;
    }

    computer_player::score_type computer_player::minimax(tictactoe::board board, int depth, bool is_maximizer, int max_depth)
    {
        const auto score = score_board(board, depth);
        // if score is 0, no one has won yet
        if (score != 0 || depth >= max_depth)
        {
            return score;
        }

        if (board.get_available_positions().empty())
        {
            return 0;
        }

        if (is_maximizer)
        {
            auto best = std::numeric_limits<score_type>::min();
            for (std::size_t row = 0; row < board.size(); row++)
            {
                for (std::size_t col = 0; col < board.size(); col++)
                {
                    if (board.read_board(row, col) == player_shape::open)
                    {
                        if (board.add_play(point(row, col), this->shape))
                        {
                            best = std::max(best, minimax(board, depth + 1, !is_maximizer, max_depth));

                        }
                        assert(board.remove_play(tictactoe::point(row, col)));

                    }
                }
            }
            return best;
        }

        // minimizer
        auto best = std::numeric_limits<score_type>::max();
        for (std::size_t row = 0; row < board.size(); row++)
        {
            for (std::size_t col = 0; col < board.size(); col++)
            {
                if (board.read_board(row, col) == player_shape::open)
                {
                    assert(board.add_play(tictactoe::point(row, col), player_shape::cross));
                    best = std::min(best, minimax(board, depth + 1, !is_maximizer, max_depth));
                    assert(board.remove_play(tictactoe::point(row, col)));
                }
            }
        }
        return best;
    }

    player_move computer_player::get_best_move(tictactoe::board& board, const player& current_player)
    {
        int best_value = std::numeric_limits<int>::min();
        tictactoe::point best_move(-1, -1);

        //available points. 
        const auto available_points = board.get_available_positions();

        // nothing we can do, return default move
        if (available_points.empty()) return {};

        //go through moves
        for (auto available_point : available_points)
        {
            auto current_play = available_point;
            //add player_move. 
            assert(board.add_play(current_play, current_player.shape));
            // const auto move_value = minimax(board, 0, false, max_depth_);
            const auto move_value = negamax(board, 1, -1000, 1000, get_opposite_shape(shape));
            //remove the player_move from the board. 
            assert(board.remove_play(available_point));

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
