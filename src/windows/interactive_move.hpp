#pragma once

#include <thread>
#include <chrono>
#include <utils/core.hpp>
#include <structures/connection.hpp>
#include <structures/attributes.hpp>
#include <structures/point.hpp>
#include <windows/attr_window.hpp>

namespace fluke {
	inline void mmove(const fluke::Connection& conn, xcb_window_t win) {
		auto [x, y, w, h, border, ignored, mapped] = fluke::window::attr(conn, win);
		xcb_warp_pointer(conn, XCB_NONE, win, 0, 0, 0, 0, w / 2, h / 2);

		auto orig_x = x, orig_y = y;

		// Grab pointer.
		uint16_t mask = XCB_EVENT_MASK_BUTTON_RELEASE | XCB_EVENT_MASK_BUTTON_RELEASE;
		auto async = XCB_GRAB_MODE_ASYNC;

		xcb_grab_pointer(conn, 0, conn.screen->root, mask, async, async, conn.screen->root, XCB_NONE, XCB_CURRENT_TIME);
		xcb_flush(conn);


		while (not xcb_poll_for_event(conn)) {

			geom = xcb_get_geometry_reply(conn,
					xcb_get_geometry(conn, win), NULL);
			if (!geom)
				errx(1, "could not get window size");


			ptr = xcb_query_pointer_reply(conn,
					xcb_query_pointer(conn, scr->root), 0);

			if (!ptr)
				errx(1, "could not get pointer location");

			values[0] = ptr->root_x + geom->width/2;
			values[1] = ptr->root_y + geom->height/2;

			values[0] = (ptr->root_x + geom->width / 2
				> scr->width_in_pixels
				- (geom->border_width*2))
				? scr->width_in_pixels - geom->width
				- (geom->border_width*2)
				: ptr->root_x - geom->width / 2;
			values[1] = (ptr->root_y + geom->height / 2
				> scr->height_in_pixels
				- (geom->border_width*2))
				? (scr->height_in_pixels - geom->height
				- (geom->border_width*2))
				: ptr->root_y - geom->height / 2;

			if (ptr->root_x < geom->width/2)
				values[0] = 0;
			if (ptr->root_y < geom->height/2)
				values[1] = 0;

			xcb_configure_window(conn, win, XCB_CONFIG_WINDOW_X
					| XCB_CONFIG_WINDOW_Y, values);
			xcb_flush(conn);
		}

		xcb_ungrab_pointer(conn, XCB_CURRENT_TIME);
	}

