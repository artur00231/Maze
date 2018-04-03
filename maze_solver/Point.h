#pragma once
#include <type_traits>
#include <utility>
#include <limits>


enum class Directions { N, E, S, W };

template<typename T>
class Point
{
public:
	Point();
	explicit Point(T x, T y = 0, T step = 0);

	Point(const Point&) = default;
	Point& operator=(const Point&) = default;

	Point(Point&&) = default;
	Point& operator=(Point&&) = default;

	~Point();

	bool operator==(const Point & point) const;
	bool operator!=(const Point & point) const;

	bool move(Directions direction, T length);
	bool move(Directions direction);
	bool move(std::pair<T, T> shift_vector);
	
	std::pair<T, T> setPosition(T x, T y);
	std::pair<T, T> setPosition(std::pair<T, T> position);
	std::pair<T, T> setPosition(const Point<T> & point);
	T setStep(T step);
	T setStep(const Point<T> & point);

	std::pair<T, T> getPosition() const;
	T getStep() const;

	static double distance(const Point<T> & point_1, const Point<T> & point_2);
	static T XY_to_index(const Point<T> & point, T width);
	static Point move(const Point<T> & point, Directions directions, T length);
	static Point move(const Point<T> & point, Directions directions);

private:
	void _move_real(Directions direction, T length);
	bool _move_integral(Directions direction, T length);

	T X{ 0 };
	T Y{ 0 };

	T step{ 0 };
};

template<typename T>
Point<T>::Point()
{
	static_assert(std::is_arithmetic_v<T>, "Inappropriate type");
	static_assert(!std::is_same_v<bool, T>, "Inappropriate type");
	static_assert(std::is_signed_v<T>, "Inappropriate type");
}

template<typename T>
Point<T>::Point(T x, T y, T step) : X{ x }, Y{ y }, step{ step }
{
	static_assert(std::is_arithmetic_v<T>, "Inappropriate type");
	static_assert(!std::is_same_v<bool, T>, "Inappropriate type");
	static_assert(std::is_signed_v<T>, "Inappropriate type");
}

template<typename T>
Point<T>::~Point()
{
}

template<typename T>
inline bool Point<T>::operator==(const Point & point) const
{
	return (getPosition() == point.getPosition());
}

template<typename T>
inline bool Point<T>::operator!=(const Point & point) const
{
	return !(this->operator==(point));
}

template<typename T>
bool Point<T>::move(Directions direction, T length)
{
	if constexpr (std::is_integral_v<T>)
	{
		return _move_integral(direction, length);
	}
	else
	{
		_move_real(direction, length);
		return true;
	}
}

template<typename T>
inline bool Point<T>::move(Directions direction)
{
	if constexpr (std::is_integral_v<T>)
	{
		return _move_integral(direction, step);
	}
	else
	{
		_move_real(direction, step);
		return true;
	}
}

template<typename T>
inline bool Point<T>::move(std::pair<T, T> shift_vector)
{
	if (shift_vector.first != 0)
	{
		move(Directions::N, shift_vector.first);
	}

	if (shift_vector.second != 0)
	{
		move(Directions::E, shift_vector.second);
	}
}

template<typename T>
std::pair<T, T> Point<T>::setPosition(T x, T y)
{
	auto x = std::make_pair(X, Y);

	X = x;
	Y = y;

	return x;
}

template<typename T>
std::pair<T, T> Point<T>::setPosition(std::pair<T, T> position)
{
	auto x = std::make_pair(X, Y);

	X = position.first;
	Y = position.second;

	return x;
}

template<typename T>
std::pair<T, T> Point<T>::setPosition(const Point<T>& point)
{
	return setStep(point.getPosition());
}

template<typename T>
T Point<T>::setStep(T step)
{
	auto x = this->step;
	this->step = step;

	return x;
}

template<typename T>
T Point<T>::setStep(const Point<T>& point)
{
	return setStep(point.getStep());
}

template<typename T>
std::pair<T, T> Point<T>::getPosition() const
{
	return { X, Y };
}

template<typename T>
T Point<T>::getStep() const
{
	return step;
}

template<typename T>
void Point<T>::_move_real(Directions direction, T length)
{
	switch (direction)
	{
	case Directions::N:
		Y += length;
		break;

	case Directions::E:
		X += length;
		break;

	case Directions::S:
		Y -= length;
		break;

	case Directions::W:
		X -= length;
		break;

	default:
		break;
	}
}

template<typename T>
bool Point<T>::_move_integral(Directions direction, T length)
{
	if (length == std::numeric_limits<T>::min())
	{
		return false;
	}

	if (length < 0)
	{
		switch (direction)
		{
		case Directions::N:
			direction = Directions::S;
			break;
		case Directions::E:
			direction = Directions::W;
			break;
		case Directions::S:
			direction = Directions::N;
			break;
		case Directions::W:
			direction = Directions::E;
			break;
		default:
			break;
		}

		length = -length;
	}

	switch (direction)
	{
	case Directions::N:
		if (Y < 0 || std::numeric_limits<T>::max() - Y > length)
		{
			Y += length;

			return true;
		}
		break;

	case Directions::E:
		if (X < 0 || std::numeric_limits<T>::max() - X > length)
		{
			X += length;

			return true;
		}
		break;

	case Directions::S:
		if (Y > 0 || std::numeric_limits<T>::min() + Y < length)
		{
			Y -= length;

			return true;
		}
		break;

	case Directions::W:
		if (X > 0 || std::numeric_limits<T>::min() + X < length)
		{
			X -= length;

			return true;
		}
		break;

	default:
		break;
	}

	return false;
}

template<typename T>
double Point<T>::distance(const Point<T> & point_1, const Point<T> & point_2)
{
	auto pos_1 = point_1.getPosition();
	auto pos_2 = point_2.getPosition();

	return std::pow(std::pow(std::abs(pos_1.first - pos_2.first), 2.0) + std::pow(std::abs(pos_1.second - pos_2.second), 2.0), 0.5);
}

template<typename T>
T Point<T>::XY_to_index(const Point<T> & point, T width)
{
	static_assert(std::is_integral_v<T>, "Inappropriate type");

	return point.getPosition().second * width + point.getPosition().first;
}

template<typename T>
inline Point<T> Point<T>::move(const Point<T>& point, Directions directions, T length)
{
	Point<T> return_point{ point };
	return_point.move(directions, length);

	return return_point;
}

template<typename T>
inline Point<T> Point<T>::move(const Point<T>& point, Directions directions)
{
	Point<T> return_point{ point };
	return_point.move(directions);

	return return_point;
}
