#pragma once

#include <utils/core.hpp>
#include <structures/connection.hpp>

namespace fluke::window {
	inline void change_focus(const fluke::Connection& conn, xcb_window_t win) {
		xcb_set_input_focus(conn, XCB_INPUT_FOCUS_POINTER_ROOT, win, XCB_CURRENT_TIME);
	}


	inline xcb_window_t current_focus(const fluke::Connection& conn) {
		auto c = xcb_get_input_focus(conn);
		auto r = xcb_get_input_focus_reply(conn, c, nullptr);

		if (not r)
			fluke::error("cannot get currently focused window.");

		xcb_window_t wid = r->focus;

		if (wid == XCB_NONE or wid == XCB_INPUT_FOCUS_POINTER_ROOT)
			fluke::error("focus not set.");

		return wid;
	}
}
