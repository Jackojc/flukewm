#pragma once

#include <iostream>
#include <fluke.hpp>

namespace fluke {
	// Aliases for some xcb types.
	namespace detail {
		using pos_type = decltype(xcb_get_geometry_reply_t::x);
		using size_type = decltype(xcb_get_geometry_reply_t::width);
	}


	// Structure that represents a rectangle: x, y, w, h.
	struct Rect {
		detail::pos_type  x, y;
		detail::size_type w, h;


		constexpr Rect():
			x{0}, y{0}, w{0}, h{0}
		{}


		template <typename T1, typename T2, typename T3, typename T4>
		explicit constexpr Rect(T1 a, T2 b, T3 c, T4 d):
			x(static_cast<detail::pos_type>(a)),
			y(static_cast<detail::pos_type>(b)),
			w(static_cast<detail::size_type>(c)),
			h(static_cast<detail::size_type>(d))
		{

		}


		// Comparison operators.
		constexpr bool operator==(const Rect& rhs) const {
			return
				x == rhs.x and
				y == rhs.y and
				w == rhs.w and
				h == rhs.h
			;
		}

		constexpr bool operator!=(const Rect& rhs) const {
			return not(*this == rhs);
		}
	};


	std::ostream& operator<<(std::ostream& os, const Rect& r) {
		auto [x, y, w, h] = r;
		return (os << '(' << x << ',' << y << ',' << w << ',' << h << ')');
	}



	// Structure that represents a point: x, y.
	struct Point {
		detail::pos_type x, y;


		constexpr Point():
			x{0}, y{0}
		{}


		template <typename T1, typename T2>
		explicit constexpr Point(T1 a, T2 b):
			x(static_cast<detail::pos_type>(a)),
			y(static_cast<detail::pos_type>(b))
		{

		}


		// Comparison operators.
		constexpr bool operator==(const Point& rhs) const {
			return
				x == rhs.x and
				y == rhs.y
			;
		}

		constexpr bool operator!=(const Point& rhs) const {
			return not(*this == rhs);
		}
	};


	std::ostream& operator<<(std::ostream& os, const Point& p) {
		auto [x, y] = p;
		return (os << '(' << x << ',' << y << ')');
	}
}

