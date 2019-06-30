#pragma once

#include <array>
#include <util/xcb.hpp>

namespace fluke {

	constexpr auto CONFIG_WINDOW_POS  = XCB_CONFIG_WINDOW_X     | XCB_CONFIG_WINDOW_Y;
	constexpr auto CONFIG_WINDOW_SIZE = XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT;
	constexpr auto CONFIG_WINDOW_RECT = XCB_CONFIG_WINDOW_X     | XCB_CONFIG_WINDOW_Y;




	struct Position {
		decltype(xcb_get_geometry_reply_t::x) x, y;
	};

	struct Size {
		decltype(xcb_get_geometry_reply_t::width) w, h;
	};

	struct Rect {
		decltype(xcb_get_geometry_reply_t::x) x, y;
		decltype(xcb_get_geometry_reply_t::width) w, h;
	};



	std::array<uint32_t, 2> to_uint32_array(const Position& pos) {
		auto [x, y] = pos;

		return {
			static_cast<uint32_t>(x),
			static_cast<uint32_t>(y)
		};
	}


	std::array<uint32_t, 2> to_uint32_array(const Size& size) {
		auto [w, h] = size;

		return {
			static_cast<uint32_t>(w),
			static_cast<uint32_t>(h)
		};
	}


	std::array<uint32_t, 4> to_uint32_array(const Rect& rect) {
		auto [x, y, w, h] = rect;

		return {
			static_cast<uint32_t>(x),
			static_cast<uint32_t>(y),
			static_cast<uint32_t>(w),
			static_cast<uint32_t>(h)
		};
	}


}
