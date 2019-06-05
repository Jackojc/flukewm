#pragma once

#include <thread>
#include <chrono>
#include <utils/core.hpp>
#include <structures/connection.hpp>
#include <structures/attributes.hpp>
#include <structures/point.hpp>
#include <windows/attr_window.hpp>

namespace fluke::window::resize {
	inline void interactive(const fluke::Connection& conn, xcb_window_t wid) {
		using namespace std::chrono_literals;

		bool topleft, topright, bottomright, bottomleft;

		auto old_x = 0, old_y = 0;

		// Stop shadowing issues by using a scope.
		{
			auto [mouse_x, mouse_y] = fluke::get_pointer_coords(conn, conn);
			auto [x, y, w, h, border, ignored, mapped] = fluke::window::attr(conn, wid);

			// Save these each run so we can see how much the window has moved.
			old_x = x;
			old_y = y;

			// Which quadrant is the pointer in?
			topleft     = mouse_x <  x + (w / 2) and mouse_y <  y + (h / 2);
			topright    = mouse_x >= x + (w / 2) and mouse_y <  y + (h / 2);
			bottomright = mouse_x >= x + (w / 2) and mouse_y >= y + (h / 2);
			bottomleft  = mouse_x <  x + (w / 2) and mouse_y >= y + (h / 2);

			// Move pointer to corner.
			auto pointer_w = (!topleft and !bottomleft) ? w : 0;
			auto pointer_h = (!topleft and !topright)   ? h : 0;
			xcb_warp_pointer(conn, XCB_NONE, wid, 0, 0, 0, 0, pointer_w, pointer_h);

			// Grab pointer.
			uint16_t mask = XCB_EVENT_MASK_BUTTON_RELEASE | XCB_EVENT_MASK_BUTTON_RELEASE;
			auto async = XCB_GRAB_MODE_ASYNC;

			xcb_grab_pointer(conn, 0, conn.screen->root, mask, async, async, conn.screen->root, XCB_NONE, XCB_CURRENT_TIME);
			xcb_flush(conn);
		}

		// Loop until an event occurs like clicking.
		while (not xcb_poll_for_event(conn)) {
			auto [mouse_x, mouse_y] = fluke::get_pointer_coords(conn, conn);
			auto [x, y, w, h, border, ignored, mapped] = fluke::window::attr(conn, wid);


			// Set the window rect depending on which corner we are resizing.
			if (topleft) {
				x = mouse_x;
				y = mouse_y;
				w = old_x - x + w;
				h = old_y - y + h;

			} else if (topright) {
				y = mouse_y;
				w = mouse_x - old_x - border * 2;
				h = old_y - y + h;

			} else if (bottomright) {
				w = mouse_x - old_x - border * 2;
				h = mouse_y - old_y - border * 2;

			} else if (bottomleft) {
				x = mouse_x;
				w = old_x - x + w;
				h = mouse_y - old_y - border * 2;
			}


			// Save old info.
			old_x = x;
			old_y = y;


			// Tell X to update the window.
			uint32_t values[] = {
				static_cast<uint32_t>(x),
				static_cast<uint32_t>(y),
				static_cast<uint32_t>(w),
				static_cast<uint32_t>(h)
			};

			uint16_t mask = 0;
			mask |= XCB_CONFIG_WINDOW_X;
			mask |= XCB_CONFIG_WINDOW_Y;
			mask |= XCB_CONFIG_WINDOW_WIDTH;
			mask |= XCB_CONFIG_WINDOW_HEIGHT;

			xcb_configure_window(conn, wid, mask, values);
			xcb_flush(conn);

			// Don't max out the cpu.
			std::this_thread::sleep_for(50ms);
		}

		// Release the cursor.
		xcb_ungrab_pointer(conn, XCB_CURRENT_TIME);
	}
}
