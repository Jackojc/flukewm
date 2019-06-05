#pragma once

#include <utils/core.hpp>
#include <structures/connection.hpp>

namespace fluke::window::map {
	inline void unmap(const fluke::Connection& conn, xcb_window_t win) {
		xcb_unmap_window(conn, win);
	}


	inline void map(const fluke::Connection& conn, xcb_window_t win) {
		xcb_map_window(conn, win);
	}


	inline void toggle(const fluke::Connection& conn, xcb_window_t win) {
		if (fluke::is_mapped(conn, win))
			xcb_unmap_window(conn, win);
		else
			xcb_map_window(conn, win);
	}
}
