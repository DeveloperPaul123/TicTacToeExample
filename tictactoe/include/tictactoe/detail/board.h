#pragma once
#include <memory>
#include <vector>

#include "tictactoe/detail/point.h"
#include "tictactoe/detail/player.h"
namespace tictactoe
{
	class board
	{
	public:
	
		using size_type = std::size_t;

		class board_callback
		{
		public:
			virtual ~board_callback() = default;
			virtual void board_changed() = 0;			
		};

		explicit board(const size_type& size);
		board(const board& other) = default;
		bool has_player_won(const player_shape& shape) const;
		bool is_game_over() const;
		std::vector<point> get_available_positions() const;
		std::vector<point> get_played_positions() const;
		bool add_play(const point& play, const player_shape& player);
		bool remove_play(const point& play);
		void clear();
		player_shape read_board(const point& pt) const;
		player_shape read_board(int row, int col) const;
		void add_callback(board_callback* callback);

	private:
		void write_board(const point& pt, const player_shape &shape) const;

		size_type size_{0};
		std::unique_ptr<int[]> board_{};
		std::vector<point> available_points_{};
		std::vector<point> played_points_{};
		std::vector<board_callback*> callbacks_;
	};
}

