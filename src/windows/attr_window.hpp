#pragma once

#include <utils/core.hpp>
#include <structures/attributes.hpp>
#include <structures/connection.hpp>

namespace fluke::window {
	inline fluke::Attributes attr(const fluke::Connection& conn, xcb_window_t win) {
		auto geom = fluke::get_window_geometry(conn, win);

		return {
			geom->x, geom->y, geom->width, geom->height,
			geom->border_width,
			fluke::is_ignored(conn, win),
			fluke::is_mapped(conn, win)
		};
	}
}
