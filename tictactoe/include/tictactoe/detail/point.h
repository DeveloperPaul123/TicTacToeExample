#pragma once

namespace tictactoe
{
	template<typename T>
	class point_t
	{
	public:
		point_t() = default;
		point_t(T row, T column)
			:row_(row), col_(column)
		{

		}

		[[nodiscard]] T row() const
		{
			return row_;
		}

		[[nodiscard]] T col() const
		{
			return col_;
		}

	private:
		T row_;
		T col_;

	};

	template<typename T>
	bool operator==(const point_t<T>& left, const point_t<T>& right)
	{
		return left.row() == right.row() && left.col() == right.col();
	}

	using point = point_t<int>;
}
