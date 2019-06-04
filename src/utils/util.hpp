#pragma once

extern "C" {
	#include <xcb/xcb.h>
	#include <xcb/xcb_aux.h>
	#include <xcb/xproto.h>
}

#include <cstdlib>
#include <cstring>

#include <utils/exception.hpp>
#include <utils/logger.hpp>

namespace fluke {
	void error(const std::string& msg) {
		throw fluke::except::Generic(msg);
	}



	void init_xcb(xcb_connection_t **con) {
		*con = xcb_connect(nullptr, nullptr);

		if (xcb_connection_has_error(*con))
			fluke::error("unable connect to the X server");
	}



	void kill_xcb(xcb_connection_t **con) {
		if (*con)
			xcb_disconnect(*con);
	}



	xcb_screen_t* get_screen(xcb_connection_t *con) {
		auto scr = xcb_setup_roots_iterator(xcb_get_setup(con)).data;

		if (scr == nullptr)
			fluke::error("unable to retrieve screen informations");

		return scr;
	}



	int exists(xcb_connection_t *con, xcb_window_t w) {
		xcb_get_window_attributes_cookie_t c;
		xcb_get_window_attributes_reply_t  *r;

		c = xcb_get_window_attributes(con, w);
		r = xcb_get_window_attributes_reply(con, c, nullptr);

		if (r == nullptr)
			return 0;

		std::free(r);
		return 1;
	}



	int mapped(xcb_connection_t *con, xcb_window_t w) {
		int map_state;

		xcb_get_window_attributes_cookie_t c;
		xcb_get_window_attributes_reply_t  *r;

		c = xcb_get_window_attributes(con, w);
		r = xcb_get_window_attributes_reply(con, c, nullptr);

		if (r == nullptr)
			return 0;

		map_state = r->map_state;

		std::free(r);
		return map_state == XCB_MAP_STATE_VIEWABLE;
	}



	int ignore(xcb_connection_t *con, xcb_window_t w) {
		int override_redirect;

		xcb_get_window_attributes_cookie_t c;
		xcb_get_window_attributes_reply_t  *r;

		c = xcb_get_window_attributes(con, w);
		r = xcb_get_window_attributes_reply(con, c, nullptr);

		if (r == nullptr)
			return 0;

		override_redirect = r->override_redirect;

		std::free(r);
		return override_redirect;
	}



	uint32_t get_windows(xcb_connection_t *con, xcb_window_t w, xcb_window_t **l) {
		uint32_t childnum = 0;
		xcb_query_tree_cookie_t c;
		xcb_query_tree_reply_t *r;

		c = xcb_query_tree(con, w);
		r = xcb_query_tree_reply(con, c, nullptr);
		if (r == nullptr)
			fluke::error("0x%08x: no such window", w);

		*l = static_cast<xcb_window_t*>(std::malloc(sizeof(xcb_window_t) * r->children_len));
		std::memcpy(*l, xcb_query_tree_children(r),
				sizeof(xcb_window_t) * r->children_len);

		childnum = r->children_len;

		std::free(r);
		return childnum;
	}
}
