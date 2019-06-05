#pragma once

#include <cstdlib>
#include <string>

#include <utils/core.hpp>
#include <structures/connection.hpp>

namespace fluke::window {
	inline void border_size(const fluke::Connection& conn, xcb_window_t win, uint16_t width) {
		auto geom = fluke::get_window_geometry(conn, win);

		// Adjust window so it remains in the same place with new border.
		uint32_t values[] = {
			static_cast<uint32_t>(geom->x + geom->border_width - width),
			static_cast<uint32_t>(geom->y + geom->border_width - width)
		};

		uint16_t mask = XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y;
		xcb_configure_window(conn, win, mask, values);

		// Set border width.
		xcb_configure_window(conn, win, XCB_CONFIG_WINDOW_BORDER_WIDTH, &width);
	}


	inline void border_colour(const fluke::Connection& conn, xcb_window_t win, const std::string& color_hex) {
		auto color = std::strtoul(color_hex.c_str(), nullptr, 16);
		xcb_change_window_attributes(conn, win, XCB_CW_BORDER_PIXEL, &color);
	}
}
