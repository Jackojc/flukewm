#pragma once

#include <utils/core.hpp>
#include <structures/connection.hpp>
#include <structures/point.hpp>

namespace fluke::mouse {
	inline void move(const fluke::Connection& conn, int x, int y) {
		xcb_warp_pointer(conn, XCB_NONE, XCB_NONE, 0, 0, 0, 0, x, y);
	}

	inline void jump(const fluke::Connection& conn, int x, int y) {
		xcb_warp_pointer(conn, XCB_NONE, conn.screen->root, 0, 0, 0, 0, x, y);
	}


	inline fluke::Point get(const fluke::Connection& conn) {
		auto cookie = xcb_query_pointer(conn, conn.screen->root);
		auto reply  = xcb_query_pointer_reply(conn, cookie, nullptr);

		if (not reply)
			fluke::error("cannot retrieve pointer position.");

		if (reply->child == XCB_NONE)
			return { reply->root_x, reply->root_y };

		return { reply->win_x, reply->win_y };
	}
}
