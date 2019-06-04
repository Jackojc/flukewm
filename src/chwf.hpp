#pragma once

#include <utils/core.hpp>

namespace fluke {
	void chwf(xcb_connection_t* conn, xcb_window_t wid) {
		xcb_set_input_focus(conn, XCB_INPUT_FOCUS_POINTER_ROOT, wid, XCB_CURRENT_TIME);
		xcb_aux_sync(conn);
	}
}
