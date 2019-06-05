#pragma once

#include <utils/core.hpp>
#include <structures/connection.hpp>

namespace fluke::window {
	inline void move(const fluke::Connection& conn, xcb_window_t wid, int16_t x, int16_t y) {
		auto cookie = xcb_get_geometry(conn, wid);
		auto reply  = xcb_get_geometry_reply(conn, cookie, nullptr);

		if (not reply)
			return;

		uint32_t values[] = {
			static_cast<uint32_t>(reply->x + x),
			static_cast<uint32_t>(reply->y + y)
		};

		uint16_t mask = 0;
		mask |= XCB_CONFIG_WINDOW_X;
		mask |= XCB_CONFIG_WINDOW_Y;

		xcb_configure_window(conn, wid, mask, values);
	}


	inline void teleport(const fluke::Connection& conn, xcb_window_t win, uint32_t x, uint32_t y, uint32_t w, uint32_t h) {
		uint16_t mask = 0;

		mask |= XCB_CONFIG_WINDOW_X;
		mask |= XCB_CONFIG_WINDOW_Y;
		mask |= XCB_CONFIG_WINDOW_WIDTH;
		mask |= XCB_CONFIG_WINDOW_HEIGHT;

		uint32_t values[] = { x, y, w, h };

		xcb_configure_window(conn, win, mask, values);
	}
}
