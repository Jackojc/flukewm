#pragma once

#include <cstdlib>
#include <string>

#include <utils/core.hpp>

namespace fluke {
	inline void chwb(xcb_connection_t* conn, uint16_t width, uint16_t color, xcb_window_t win) {
		xcb_get_geometry_reply_t* geom = xcb_get_geometry_reply(conn, xcb_get_geometry(conn, win), nullptr);


		if (not geom)
			return;


		uint32_t values[2]; // this _must_ be uint32_t.
		uint16_t curr_width;
		uint16_t mask;


		// Adjust window so it remains in the same place with new border.
		curr_width = geom->border_width;
		values[0]  = static_cast<uint32_t>(geom->x + curr_width - width);
		values[1]  = static_cast<uint32_t>(geom->y + curr_width - width);

		mask = XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y;
		xcb_configure_window(conn, win, mask, values);


		// Set border width.
		mask = XCB_CONFIG_WINDOW_BORDER_WIDTH;
		xcb_configure_window(conn, win, mask, &width);


		// Colour
		mask = XCB_CW_BORDER_PIXEL;
		xcb_change_window_attributes(conn, win, mask, &color);


		// Sync changes.
		xcb_aux_sync(conn);
		std::free(geom);
	}
}
