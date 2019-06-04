#pragma once

#include <string>
#include <utils/core.hpp>

namespace fluke {
	void chwn(xcb_connection_t* conn, xcb_window_t wid, const std::string& name) {
		xcb_change_property(
			conn,
			XCB_PROP_MODE_REPLACE,
			wid,
			XCB_ATOM_WM_NAME,
			XCB_ATOM_STRING,
			8, // format, 8bit chars.
			name.size(),
			name.c_str()
		);

		xcb_flush(conn);
	}
}
