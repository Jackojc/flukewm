#pragma once

#include <utils/core.hpp>
#include <structures/connection.hpp>

namespace fluke::window::ignore {
	inline void ignore(const fluke::Connection& conn, xcb_window_t wid) {
		uint32_t val = 1;
		xcb_change_window_attributes(conn, wid, XCB_CW_OVERRIDE_REDIRECT, &val);
	}


	inline void notice(const fluke::Connection& conn, xcb_window_t wid) {
		uint32_t val = 0;
		xcb_change_window_attributes(conn, wid, XCB_CW_OVERRIDE_REDIRECT, &val);
	}
}
