#pragma once

#include <utils/core.hpp>
#include <structures/connection.hpp>

namespace fluke::window::stack {
	inline void above(const fluke::Connection& conn, xcb_window_t win) {
		uint32_t order = XCB_STACK_MODE_ABOVE;
		xcb_configure_window(conn, win, XCB_CONFIG_WINDOW_STACK_MODE, &order);
	}


	inline void below(const fluke::Connection& conn, xcb_window_t win) {
		uint32_t order = XCB_STACK_MODE_BELOW;
		xcb_configure_window(conn, win, XCB_CONFIG_WINDOW_STACK_MODE, &order);
	}


	inline void opposite(const fluke::Connection& conn, xcb_window_t win) {
		uint32_t order = XCB_STACK_MODE_OPPOSITE;
		xcb_configure_window(conn, win, XCB_CONFIG_WINDOW_STACK_MODE, &order);
	}
}
