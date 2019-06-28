#pragma once

#include <array>
#include <util/xcb.hpp>

namespace fluke {

	constexpr auto CONFIG_WINDOW_POS  = XCB_CONFIG_WINDOW_X     | XCB_CONFIG_WINDOW_Y;
	constexpr auto CONFIG_WINDOW_SIZE = XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT;
	constexpr auto CONFIG_WINDOW_RECT = XCB_CONFIG_WINDOW_X     | XCB_CONFIG_WINDOW_Y;



	struct WindowPosition {
		using pos_type = decltype(xcb_get_geometry_reply_t::x);
		std::array<uint32_t, 2> pos;

		WindowPosition(pos_type x, pos_type y) {
			pos.at(0) = static_cast<uint32_t>(x);
			pos.at(1) = static_cast<uint32_t>(y);
		}

		operator uint32_t* () const {
			return pos.data();
		}
	};


	struct WindowSize {
		using size_type = decltype(xcb_get_geometry_reply_t::width);
		std::array<uint32_t, 2> size;

		WindowSize(size_type w, size_type h) {
			size.at(0) = static_cast<uint32_t>(w);
			size.at(1) = static_cast<uint32_t>(h);
		}

		operator uint32_t* () const {
			return size.data();
		}
	};


	struct WindowRect {
		using size_type = decltype(xcb_get_geometry_reply_t::width);
		using pos_type  = decltype(xcb_get_geometry_reply_t::x);
		std::array<uint32_t, 4> rect;

		WindowRect(pos_type x, pos_type y, size_type w, size_type h) {
			rect.at(0) = static_cast<uint32_t>(x);
			rect.at(1) = static_cast<uint32_t>(y);
			rect.at(2) = static_cast<uint32_t>(w);
			rect.at(3) = static_cast<uint32_t>(h);
		}

		operator uint32_t* () const {
			return rect.data();
		}
	};



}
