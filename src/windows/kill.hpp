#pragma once

#include <utils/core.hpp>
#include <structures/connection.hpp>

namespace fluke::window {
	inline void kill(const fluke::Connection& conn, xcb_window_t wid, bool parent = false) {
		if (parent) xcb_kill_client(conn, wid);
		else        xcb_destroy_window(conn, wid);
	}


	inline void close(const fluke::Connection& conn, xcb_window_t wid) {
		// Just cache some atom info.
		auto atom_proto = fluke::atom_get(conn, "WM_PROTOCOLS");
		auto atom_del   = fluke::atom_get(conn, "WM_DELETE_WINDOW");

		auto cookie = xcb_icccm_get_wm_protocols_unchecked(conn, wid, atom_proto);
		xcb_icccm_get_wm_protocols_reply_t reply;


		// If window doesn't care about EWMH, just close it.
		if (not xcb_icccm_get_wm_protocols_reply(conn, cookie, &reply, nullptr))
			xcb_kill_client(conn, wid);


		// Otherwise it must be EWMH compliant.
		xcb_client_message_event_t ev;

		ev.response_type  = XCB_CLIENT_MESSAGE;
		ev.sequence       = 0;
		ev.format         = 32;
		ev.window         = wid;
		ev.type           = atom_proto;
		ev.data.data32[0] = atom_del;
		ev.data.data32[1] = XCB_CURRENT_TIME;

		xcb_send_event(conn, 0, wid, XCB_EVENT_MASK_NO_EVENT, reinterpret_cast<char*>(&ev));
	}
}
