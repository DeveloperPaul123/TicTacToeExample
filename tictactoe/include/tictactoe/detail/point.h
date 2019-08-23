#pragma once

namespace tictactoe
{
	template<typename T>
	class point_t
	{
	public:
		point_t() = default;
		point_t(T x, T y)
			:x_(x), y_(y)
		{

		}
		T x() const
		{
			return x_;
		}
		T y() const
		{
			return y_;
		}

	private:
		T x_;
		T y_;

	};

	template<typename T>
	bool operator==(const point_t<T>& left, const point_t<T>& right)
	{
		return left.x() == right.x() && left.y() == right.y();
	}

	using point = point_t<int>;
}
