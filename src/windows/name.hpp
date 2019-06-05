#pragma once

#include <string>
#include <utils/core.hpp>
#include <structures/connection.hpp>

namespace fluke::window::name {
	// Set window name.
	inline void set(const fluke::Connection& conn, xcb_window_t wid, const std::string& name) {
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


	// Get window name.
	inline std::string get(xcb_connection_t* conn, xcb_window_t win) {
		auto cookie = xcb_get_property(conn, 0, win, XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 0L, 32L);
		auto reply  = xcb_get_property_reply(conn, cookie, NULL);

		if (not reply)
			fluke::error("could not get window title.");

		auto name = xcb_get_property_value(reply);
		auto size = xcb_get_property_value_length(reply);

		return {reinterpret_cast<char*>(name), static_cast<std::string::size_type>(size)};
	}
}
