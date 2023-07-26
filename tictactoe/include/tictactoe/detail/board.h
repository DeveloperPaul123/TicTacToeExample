#pragma once
#include <functional>
#include <memory>
#include <vector>
#include <concepts>

#include "tictactoe/detail/point.h"
#include "tictactoe/detail/player.h"

namespace tictactoe
{
	class board
	{
	public:

		using size_type = std::size_t;
		explicit board(const size_type& single_dimension_size = 3);
		board(std::initializer_list<player_shape> init_list);
		board(const board& other);
		board(board&& other) noexcept;
		board& operator=(const board& other);
		board& operator=(board&& other) noexcept;
		[[nodiscard]] bool has_player_won(const player_shape& shape) const;
		[[nodiscard]] bool is_game_over() const;
		[[nodiscard]] std::vector<point> get_available_positions() const;
		[[nodiscard]] std::vector<point> get_played_positions() const;
		[[nodiscard]] bool add_play(const point& play, const player_shape& player) const;
		[[nodiscard]] bool remove_play(const point& play) const;
		void clear() const;
		[[nodiscard]] player_shape read_board(const point& pt) const;
		[[nodiscard]] player_shape read_board(size_type row, size_type col) const;

		template<typename Callback>
			requires std::invocable<Callback>
		void add_board_change_callback(Callback&& callback)
		{
			callbacks_.push_back(std::forward<Callback>(callback));
		}

		[[nodiscard]] size_type size() const;
		[[nodiscard]] size_type grid_size() const;

	private:
		void write_board(const point& pt, const player_shape& shape) const;
		using callback = std::function<void()>;
		size_type size_{ 0 };
		std::unique_ptr<int[]> board_{};
		std::vector<callback> callbacks_;
	};
}

