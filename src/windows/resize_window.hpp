#pragma once

#include <utils/core.hpp>
#include <structures/connection.hpp>
#include <windows/attr_window.hpp>

namespace fluke::window::resize {
	inline void grow(const fluke::Connection& conn, xcb_window_t win, int16_t horz, int16_t vert) {
		auto [x, y, w, h, border, ignored, mapped] = fluke::window::attr(conn, win);

		x -= horz;
		y -= vert;
		w += horz * 2;
		h += vert * 2;

		uint32_t mask = 0;
		mask |= XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y;
		mask |= XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT;

		uint32_t values[] = {
			static_cast<uint32_t>(x),
			static_cast<uint32_t>(y),
			static_cast<uint32_t>(w),
			static_cast<uint32_t>(h)
		};

		xcb_configure_window(conn, win, mask, values);
	}




	inline void resize_topleft(const fluke::Connection& conn, xcb_window_t win, int16_t horz, int16_t vert) {
		auto [x, y, w, h, border, ignored, mapped] = fluke::window::attr(conn, win);

		x -= horz;
		y -= vert;
		w += horz;
		h += vert;

		uint32_t mask = 0;
		mask |= XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y;
		mask |= XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT;

		uint32_t values[] = {
			static_cast<uint32_t>(x),
			static_cast<uint32_t>(y),
			static_cast<uint32_t>(w),
			static_cast<uint32_t>(h)
		};

		xcb_configure_window(conn, win, mask, values);
	}





	inline void resize_topright(const fluke::Connection& conn, xcb_window_t win, int16_t horz, int16_t vert) {
		auto [x, y, w, h, border, ignored, mapped] = fluke::window::attr(conn, win);

		y += vert;
		w += horz;
		h += vert;

		uint32_t mask = 0;
		mask |= XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y;
		mask |= XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT;

		uint32_t values[] = {
			static_cast<uint32_t>(x),
			static_cast<uint32_t>(y),
			static_cast<uint32_t>(w),
			static_cast<uint32_t>(h)
		};

		xcb_configure_window(conn, win, mask, values);
	}





	inline void resize_bottomright(const fluke::Connection& conn, xcb_window_t win, int16_t horz, int16_t vert) {
		auto [x, y, w, h, border, ignored, mapped] = fluke::window::attr(conn, win);

		w += horz;
		h += vert;

		uint32_t mask = 0;
		mask |= XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y;
		mask |= XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT;

		uint32_t values[] = {
			static_cast<uint32_t>(x),
			static_cast<uint32_t>(y),
			static_cast<uint32_t>(w),
			static_cast<uint32_t>(h)
		};

		xcb_configure_window(conn, win, mask, values);
	}





	inline void resize_bottomleft(const fluke::Connection& conn, xcb_window_t win, int16_t horz, int16_t vert) {
		auto [x, y, w, h, border, ignored, mapped] = fluke::window::attr(conn, win);

		x -= horz;
		w += horz;
		h += vert;

		uint32_t mask = 0;
		mask |= XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y;
		mask |= XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT;

		uint32_t values[] = {
			static_cast<uint32_t>(x),
			static_cast<uint32_t>(y),
			static_cast<uint32_t>(w),
			static_cast<uint32_t>(h)
		};

		xcb_configure_window(conn, win, mask, values);
	}
}
