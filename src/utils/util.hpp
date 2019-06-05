#pragma once

extern "C" {
	#include <xcb/xcb.h>
	#include <xcb/xcb_aux.h>
	#include <xcb/xproto.h>
	#include <xcb/xcb_icccm.h>
}

#include <cstdlib>
#include <cstring>

#include <utils/exception.hpp>
#include <utils/logger.hpp>

#include <structures/point.hpp>

namespace fluke {
	void error(const std::string& msg) {
		throw fluke::except::Generic(msg);
	}



	template <typename T, typename... Ts>
	inline bool fold_or(T arg, Ts&&... args) {
		return ((arg != args) or ...);
	}



	xcb_atom_t atom_get(xcb_connection_t *conn, const std::string& name) {
		auto cookie = xcb_intern_atom(conn, 0, name.size(), name.c_str());
		auto reply  = xcb_intern_atom_reply(conn, cookie, nullptr);

		if (not reply)
			return XCB_ATOM_STRING;

		return reply->atom;
	}



	fluke::Point get_pointer_coords(xcb_connection_t* conn, xcb_screen_t* screen) {
		auto mouse = xcb_query_pointer_reply(conn, xcb_query_pointer(conn, screen->root), nullptr);

		if (not mouse)
			fluke::error("could not get mouse location");

		return { mouse->root_x, mouse->root_y };
	}




	void init_xcb(xcb_connection_t **conn) {
		*conn = xcb_connect(nullptr, nullptr);

		if (xcb_connection_has_error(*conn))
			fluke::error("unable connect to the X server");

		xcb_aux_sync(*conn);
	}



	void kill_xcb(xcb_connection_t **conn) {
		xcb_aux_sync(*conn);

		if (*conn)
			xcb_disconnect(*conn);
	}



	xcb_screen_t* get_screen(xcb_connection_t *conn) {
		auto scr = xcb_setup_roots_iterator(xcb_get_setup(conn)).data;

		if (scr == nullptr)
			fluke::error("unable to retrieve screen informations");

		return scr;
	}



	int exists(xcb_connection_t *conn, xcb_window_t w) {
		xcb_get_window_attributes_cookie_t c;
		xcb_get_window_attributes_reply_t  *r;

		c = xcb_get_window_attributes(conn, w);
		r = xcb_get_window_attributes_reply(conn, c, nullptr);

		if (r == nullptr)
			return 0;

		return 1;
	}



	int is_mapped(xcb_connection_t *conn, xcb_window_t w) {
		int map_state;

		xcb_get_window_attributes_cookie_t c;
		xcb_get_window_attributes_reply_t  *r;

		c = xcb_get_window_attributes(conn, w);
		r = xcb_get_window_attributes_reply(conn, c, nullptr);

		if (r == nullptr)
			return 0;

		map_state = r->map_state;

		return map_state == XCB_MAP_STATE_VIEWABLE;
	}



	int is_ignored(xcb_connection_t *conn, xcb_window_t w) {
		int override_redirect;

		xcb_get_window_attributes_cookie_t c;
		xcb_get_window_attributes_reply_t  *r;

		c = xcb_get_window_attributes(conn, w);
		r = xcb_get_window_attributes_reply(conn, c, nullptr);

		if (r == nullptr)
			return 0;

		override_redirect = r->override_redirect;

		return override_redirect;
	}



	auto get_windows(xcb_connection_t* conn, xcb_window_t wid) {
		using vec_t = std::vector<xcb_window_t>;

		auto cookie = xcb_query_tree(conn, wid);
		auto reply  = xcb_query_tree_reply(conn, cookie, nullptr);

		if (not reply)
			fluke::error("no such window: ", wid);


		vec_t vec;

		auto size = static_cast<vec_t::size_type>(reply->children_len);
		vec.resize(size);


		auto ptr = xcb_query_tree_children(reply);
		std::copy(ptr, ptr + size, vec.begin());


		return vec;
	}
}
