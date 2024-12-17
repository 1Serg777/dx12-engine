#pragma once

#include <cstdint>

namespace dxe
{
	// Coordinate2D

	template <typename T>
	struct Coordinate2D
	{
		constexpr Coordinate2D() = default;
		constexpr Coordinate2D(T x, T y)
			: x(x), y(y) {}

		T x{ 0 };
		T y{ 0 };
	};

	template<typename T>
	inline bool operator==(const Coordinate2D<T>& c1, const Coordinate2D<T>& c2)
	{
		return ((c1.x == c2.x) && (c1.y == c2.y));
	}
	template<typename T>
	inline bool operator!=(const Coordinate2D<T>& c1, const Coordinate2D<T>& c2)
	{
		return !(c1 == c2);
	}
	template<typename T>
	inline bool operator<(const Coordinate2D<T>& c1, const Coordinate2D<T>& c2)
	{
		return ((c1.x < c2.x) && (c1.y < c2.y));
	}
	template<typename T>
	inline bool operator<=(const Coordinate2D<T>& c1, const Coordinate2D<T>& c2)
	{
		return ((c1 == c2) || (c1 < c2));
	}
	template<typename T>
	inline bool operator>(const Coordinate2D<T>& c1, const Coordinate2D<T>& c2)
	{
		return ((c1.x > c2.x) && (c1.y > c2.y));
	}
	template<typename T>
	inline bool operator>=(const Coordinate2D<T>& c1, const Coordinate2D<T>& c2)
	{
		return ((c1 == c2) || (c1 > c2));
	}

	template<typename T>
	inline Coordinate2D<T> operator+(const Coordinate2D<T>& c1, const Coordinate2D<T>& c2)
	{
		return Coordinate2D<T>{ c1.x + c2.x, c1.y + c2.y };
	}

	// Dimensions2D

	struct Dimensions2D
	{
		constexpr Dimensions2D() = default;
		constexpr Dimensions2D(uint32_t width, uint32_t height)
			: width(width), height(height) {}

		inline uint32_t Area() const { return width * height; }
		inline float AspectRatio() const { return static_cast<float>(width) / height; }

		template <typename T>
		inline bool CheckCoordinateFit(const Coordinate2D<T>& coordinate) const;

		uint32_t width{ 0 };
		uint32_t height{ 0 };
	};

	inline bool operator==(const Dimensions2D& d1, const Dimensions2D& d2)
	{
		return ((d1.width == d2.width) && (d1.height == d2.height));
	}
	inline bool operator!=(const Dimensions2D& d1, const Dimensions2D& d2)
	{
		return !(d1 == d2);
	}
	inline bool operator<(const Dimensions2D& d1, const Dimensions2D& d2)
	{
		return ((d1.width < d2.width) && (d1.height < d2.height));
	}
	inline bool operator<=(const Dimensions2D& d1, const Dimensions2D& d2)
	{
		return ((d1.width <= d2.width) && (d1.height <= d2.height));
	}
	inline bool operator>(const Dimensions2D& d1, const Dimensions2D& d2)
	{
		return ((d1.width > d2.width) && (d1.height > d2.height));
	}
	inline bool operator>=(const Dimensions2D& d1, const Dimensions2D& d2)
	{
		return ((d1.width >= d2.width) && (d1.height >= d2.height));
	}

	template<typename T>
	inline bool Dimensions2D::CheckCoordinateFit(const Coordinate2D<T>& coordinate) const
	{
		return ((coordinate.x >= 0) && (coordinate.x <= width)) &&
			((coordinate.y >= 0) && (coordinate.y <= height));
	}
}