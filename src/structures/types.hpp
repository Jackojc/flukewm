#pragma once

#include <iostream>
#include <fluke.hpp>

namespace fluke {
	namespace detail {
		using pos_type = decltype(xcb_get_geometry_reply_t::x);
		using size_type = decltype(xcb_get_geometry_reply_t::width);
	}

	struct Rect {
		detail::pos_type  x, y;
		detail::size_type w, h;

		template <typename T1, typename T2, typename T3, typename T4>
		Rect(T1 a, T2 b, T3 c, T4 d):
			x(static_cast<detail::pos_type>(a)),
			y(static_cast<detail::pos_type>(b)),
			w(static_cast<detail::size_type>(c)),
			h(static_cast<detail::size_type>(d))
		{

		}
	};

	std::ostream& operator<<(std::ostream& os, const Rect& r) {
		auto [x, y, w, h] = r;
		return (os << '(' << x << ',' << y << ',' << w << ',' << h << ')');
	}




	struct Point {
		detail::pos_type x, y;

		template <typename T1, typename T2>
		Point(T1 a, T2 b):
			x(static_cast<detail::pos_type>(a)),
			y(static_cast<detail::pos_type>(b))
		{

		}
	};

	std::ostream& operator<<(std::ostream& os, const Point& p) {
		auto [x, y] = p;
		return (os << '(' << x << ',' << y << ')');
	}
}

