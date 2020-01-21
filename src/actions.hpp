#pragma once

#include <algorithm>
#include <optional>
#include <tuple>
#include <cmath>
#include <fluke.hpp>

namespace fluke {
	// Move, Resize, Grow, Shrink
	inline void action_resize(fluke::Connection& conn, int x_amount, int y_amount, int w_amount, int h_amount) {
		FLUKE_DEBUG_NOTICE(
			"action '", tinge::fg::make_yellow("RESIZE/MOVE"),
			"' with arg(s) '",
			tinge::fg::make_yellow("x: ", x_amount),
			tinge::fg::make_yellow(", y: ", y_amount),
			tinge::fg::make_yellow(", w: ", w_amount),
			tinge::fg::make_yellow(", h: ", h_amount),
			"'"
		)

		// Get the currently focused window.
		FLUKE_DEBUG_NOTICE_SUB("get focused window.");
		const xcb_window_t focused = fluke::get_focused_window(conn);


		// Get geometry of focused window and calculate offsets for the new window rect.
		FLUKE_DEBUG_NOTICE_SUB("calculate offsets.");
		const auto [x, y, w, h] = fluke::as_rect(fluke::get(conn, fluke::get_geometry(conn, focused)));

		const auto new_x = static_cast<uint32_t>(x + x_amount);
		const auto new_y = static_cast<uint32_t>(y + y_amount);
		const auto new_w = static_cast<uint32_t>(w + w_amount);
		const auto new_h = static_cast<uint32_t>(h + h_amount);

		FLUKE_DEBUG_NOTICE_SUB("set new rect for window.");
		fluke::configure_window(conn, focused, fluke::XCB_MOVE_RESIZE, new_x, new_y, new_w, new_h);
	}


	// Change workspace
	inline void action_workspace_change(fluke::Connection&) {

	}

	inline void action_workspace_send_to(fluke::Connection&) {

	}





	// scratchpad
	inline void action_scratchpad_show(fluke::Connection&) {

	}

	inline void action_scratchpad_hide(fluke::Connection&) {

	}

	inline void action_scratchpad_toggle(fluke::Connection&) {

	}

	inline void action_scratchpad_add(fluke::Connection&) {

	}

	inline void action_scratchpad_remove(fluke::Connection&) {

	}







	// Alt tab like focusing
	enum {
		FOCUS_LEFT,
		FOCUS_RIGHT,
		FOCUS_UP,
		FOCUS_DOWN,
	};


	constexpr const char* focus_dir_str[] = {
		"FOCUS_LEFT",
		"FOCUS_RIGHT",
		"FOCUS_UP",
		"FOCUS_DOWN",
	};

	inline void action_focus_dir(fluke::Connection& conn, int dir) {
		FLUKE_DEBUG_NOTICE(
			"action '", tinge::fg::make_yellow("FOCUS_DIR"),
			"' with arg(s) '", tinge::fg::make_yellow(focus_dir_str[dir]), "'"
		)

		// Get the window which currently has keyboard focus.
		FLUKE_DEBUG_NOTICE_SUB("get focused window.");
		const xcb_window_t focused = fluke::get_focused_window(conn);

		// Vector of all windows currently mapped.
		const auto windows = fluke::get_mapped_windows(conn);

		if (windows.size() <= 1)
			return;

		// Get the geometry of all mapped windows.
		// We also want to find the index for the geometry of the focused
		// specifically too.
		FLUKE_DEBUG_NOTICE_SUB("get geometry of all mapped windows.");
		decltype(windows)::size_type i = 0;
		const auto geoms = fluke::dispatch_consume(conn, [&] (xcb_window_t win) {
			if (win != focused)
				i++;

			return fluke::get_geometry( conn, win );
		}, windows);


		// Get the midpoint of each side of a rectangle.
		const auto left_side   = [] (auto x, auto y, auto w, auto h) { return fluke::Point{ x,         y + h / 2 }; };
		const auto right_side  = [] (auto x, auto y, auto w, auto h) { return fluke::Point{ x + w,     y + h / 2 }; };
		const auto top_side    = [] (auto x, auto y, auto w, auto h) { return fluke::Point{ x + w / 2, y         }; };
		const auto bottom_side = [] (auto x, auto y, auto w, auto h) { return fluke::Point{ x + w / 2, y + h     }; };


		// Get geometry of currently focused window.
		FLUKE_DEBUG_NOTICE_SUB("get reference point on focused window.");
		const auto [fx, fy, fw, fh] = fluke::as_rect(geoms.at(i));

		// Get the midpoint of the relevant side of the currently focused window.
		const std::array fsides{
			left_side(fx, fy, fw, fh),
			right_side(fx, fy, fw, fh),
			top_side(fx, fy, fw, fh),
			bottom_side(fx, fy, fw, fh),
		};

		const fluke::Point fpoint = fsides.at(static_cast<decltype(fsides)::size_type>(dir));


		// Store distances to windows and the associated window IDs.
		std::vector<std::pair<xcb_window_t, int>> distances;

		// Get the distance from center point of focused window to another point.
		const auto distance = [&fpoint] (const fluke::Point& p) {
			return std::sqrt(std::pow(fpoint.x - p.x, 2) + std::pow(fpoint.y - p.y, 2));
		};


		// Loop over all windows and get the midpoint of all of their four sides.
		// We get the point which is nearest to the focused window and
		// add it to our `distances` vector.
		FLUKE_DEBUG_NOTICE_SUB("find nearest points on other windows to reference point.");
		for (const auto [win, geom]: fluke::zip(windows, geoms)) {
			if (win == focused)
				continue;

			const auto [x, y, w, h] = fluke::as_rect(geom);

			// Find which side is closest to focused window.
			const std::array sides{
				distance(left_side(x, y, w, h)),
				distance(right_side(x, y, w, h)),
				distance(top_side(x, y, w, h)),
				distance(bottom_side(x, y, w, h)),
			};

			distances.emplace_back(win, *std::min_element(sides.begin(), sides.end()));
		}

		// Find window with nearest distance.
		FLUKE_DEBUG_NOTICE_SUB("get nearest window.");
		const auto nearest_win = std::min_element(distances.begin(), distances.end(), [] (const auto& a, const auto& b) {
			return a.second < b.second;
		})->first;

		// Set input focus to new window.
		FLUKE_DEBUG_NOTICE_SUB("focus window.");
		fluke::configure_window(conn, nearest_win, XCB_CONFIG_WINDOW_STACK_MODE, XCB_STACK_MODE_ABOVE);
		fluke::set_input_focus(conn, XCB_INPUT_FOCUS_POINTER_ROOT, nearest_win);
	}



	enum {
		FOCUS_PREV,
		FOCUS_NEXT,
	};

	constexpr const char* focus_str[] = {
		"FOCUS_PREV",
		"FOCUS_NEXT",
	};

	inline void action_focus(fluke::Connection& conn, int dir) {
		FLUKE_DEBUG_NOTICE(
			"action '", tinge::fg::make_yellow("FOCUS"),
			"' with arg(s) '", tinge::fg::make_yellow(focus_str[dir]), "'"
		)

		// Get all of the mapped windows.
		const auto windows = fluke::get_mapped_windows(conn);

		if (windows.size() <= 1)
			return;

		// Get the currently focused window.
		FLUKE_DEBUG_NOTICE_SUB("get focused window.");
		const xcb_window_t focused = fluke::get_focused_window(conn);

		// Depending on which direction we are focusing, we need to use
		// different options and windows.
		FLUKE_DEBUG_NOTICE_SUB("generate table for options.");
		const std::array opts {
			std::pair{windows.back(), XCB_STACK_MODE_BELOW},  // Previous
			std::pair{windows.front(), XCB_STACK_MODE_ABOVE}, // Next
		};

		FLUKE_DEBUG_NOTICE_SUB("focus window.");
		const auto[next_win, stack_mode] = opts.at(static_cast<decltype(opts)::size_type>(dir));

		// Set focus to new window and shuffle the window stack around.
		fluke::configure_window(conn, focused, XCB_CONFIG_WINDOW_STACK_MODE, stack_mode);
		fluke::configure_window(conn, next_win, XCB_CONFIG_WINDOW_STACK_MODE, stack_mode);
		fluke::set_input_focus(conn, XCB_INPUT_FOCUS_PARENT, next_win);
	}








	// Misc actions
	inline void action_center(fluke::Connection& conn) {
		FLUKE_DEBUG_NOTICE("action '", tinge::fg::make_yellow("CENTER"), "'")

		// Get the focused window and its geometry.
		FLUKE_DEBUG_NOTICE_SUB("get focused window and display.");
		const xcb_window_t focused = fluke::get_focused_window(conn);
		const auto focused_rect = fluke::as_rect(fluke::get(conn, fluke::get_geometry(conn, focused)));

		// Get rect of focused display.
		const auto [window_x, window_y, window_w, window_h] = focused_rect;
		const auto [display_x, display_y, display_w, display_h] = fluke::get_nearest_display_rect(conn, focused_rect);

		// Center the window on the screen.
		const auto x = (display_x + display_w / 2) - window_w / 2;
		const auto y = (display_y + display_h / 2) - window_h / 2;

		FLUKE_DEBUG_NOTICE_SUB("center window.");
		fluke::configure_window(conn, focused, fluke::XCB_MOVE, x, y);
	}








	// Window snapping
	enum {
		SNAP_LEFT,
		SNAP_RIGHT,
		SNAP_TOP,
		SNAP_BOTTOM,
	};

	constexpr const char* side_str[] = {
		"SNAP_LEFT",
		"SNAP_RIGHT",
		"SNAP_TOP",
		"SNAP_BOTTOM",
	};

	inline void action_snap_side(fluke::Connection& conn, int side) {
		FLUKE_DEBUG_NOTICE(
			"action '", tinge::fg::make_yellow("SNAP_SIDE"),
			"' with arg(s) '", tinge::fg::make_yellow(side_str[side]), "'"
		)

		// Get focused window.
		FLUKE_DEBUG_NOTICE_SUB("getting focused window and display.")
		const xcb_window_t focused = fluke::get_focused_window(conn);
		const auto focused_rect = fluke::as_rect(fluke::get(conn, fluke::get_geometry(conn, focused)));

		// Get focused display.
		const auto [window_x, window_y, window_w, window_h] = focused_rect;
		auto [display_x, display_y, display_w, display_h] = fluke::get_nearest_display_rect(conn, focused_rect);


		// Get usable screen area.
		FLUKE_DEBUG_NOTICE_SUB("calculate offsets.");
		display_x += fluke::config::GAP + fluke::config::GUTTER_LEFT;
		display_y += fluke::config::GAP + fluke::config::GUTTER_TOP;
		display_w -= fluke::config::BORDER_SIZE * 2 + fluke::config::GAP * 2 + fluke::config::GUTTER_RIGHT;
		display_h -= fluke::config::BORDER_SIZE * 2 + fluke::config::GAP * 2 + fluke::config::GUTTER_BOTTOM;


		// Rectangles that window will be moved into.
		const std::array side_rects {
			// Left side.
			fluke::Rect{
				display_x,
				display_y,
				display_w / 2 - fluke::config::BORDER_SIZE * 2,
				display_h
			},

			// Right side.
			fluke::Rect{
				display_x + display_w / 2 + fluke::config::GAP * 2,
				display_y,
				display_w / 2 - fluke::config::BORDER_SIZE * 2,
				display_h
			},

			// Top side.
			fluke::Rect{
				display_x,
				display_y,
				display_w,
				display_h / 2 - fluke::config::BORDER_SIZE * 2
			},

			// Bottom side.
			fluke::Rect{
				display_x,
				display_y + display_h / 2 + fluke::config::GAP * 2,
				display_w,
				display_h / 2 - fluke::config::BORDER_SIZE * 2
			},
		};

		FLUKE_DEBUG_NOTICE_SUB("snap window.");

		// Get the rect of the side we wish to move our window into.
		const auto [x, y, w, h] = side_rects.at(static_cast<decltype(side_rects)::size_type>(side));
		fluke::configure_window(conn, focused, fluke::XCB_MOVE_RESIZE, x, y, w, h);
	}



	inline void action_snap_corner(fluke::Connection&) {

	}










	// Tiling
	enum {
		MASTER_LEFT,
		MASTER_RIGHT,
	};

	constexpr const char* master_str[] = {
		"MASTER_LEFT",
		"MASTER_RIGHT",
	};

	inline void action_layout_masterslave(fluke::Connection& conn, int master_side) {
		FLUKE_DEBUG_NOTICE(
			"action '", tinge::fg::make_yellow("LAYOUT_MASTERSLAVE"),
			"' with arg(s) '", tinge::fg::make_yellow(master_str[master_side]), "'"
		)

		// Get all of the mapped windows.
		FLUKE_DEBUG_NOTICE_SUB("getting mapped windows.")
		auto windows = fluke::get_mapped_windows(conn);

		if (windows.size() <= 1)
			return;


		// Get focused window ID.
		FLUKE_DEBUG_NOTICE_SUB("getting the focused window.")
		const xcb_window_t focused = fluke::get_focused_window(conn);


		// Get the geometry of all mapped windows.
		// Find the index of the focused window also.
		FLUKE_DEBUG_NOTICE_SUB("getting the window geometry for all windows.")
		decltype(windows)::size_type focused_index = 0;
		const auto geoms = fluke::dispatch_consume(conn, [&] (xcb_window_t win) {
			if (win != focused)
				focused_index++;

			return fluke::get_geometry( conn, win );
		}, windows);


		// Get focused window rect.
		FLUKE_DEBUG_NOTICE_SUB("getting nearest display to focused window.")
		const auto focused_rect = fluke::as_rect(geoms.at(focused_index));

		// Get the rect of the nearest display to the focused window.
		auto display_rect = fluke::get_nearest_display_rect(conn, focused_rect);
		auto [display_x, display_y, display_w, display_h] = display_rect;


		// Filter out windows which are not on the same display as the focused window.
		FLUKE_DEBUG_NOTICE_SUB("filter windows outside of focused display.")
		decltype(windows)::size_type geoms_index = 0;
		windows.erase(std::remove_if(windows.begin(), windows.end(), [&] (xcb_window_t win) {
			return display_rect != fluke::get_nearest_display_rect(conn, fluke::as_rect(geoms.at(geoms_index++)));
		}), windows.end());



		// Get the height that each slave window should be.
		FLUKE_DEBUG_NOTICE_SUB("calculating offsets.")
		const int slave_height = (display_h / (windows.size() - 1)) - (fluke::config::GAP * 2 - fluke::config::BORDER_SIZE * 2);

		// Get the usable display area.
		// Leave a gap for window borders.
		display_x += fluke::config::GAP + fluke::config::GUTTER_LEFT;
		display_y += fluke::config::GAP + fluke::config::GUTTER_TOP;
		display_w -= fluke::config::BORDER_SIZE * 2 + fluke::config::GAP * 2 + fluke::config::GUTTER_RIGHT;
		display_h -= fluke::config::BORDER_SIZE * 2 + fluke::config::GAP * 2 + fluke::config::GUTTER_BOTTOM;


		int sliding_y = display_y;  // Keep track of Y position so we can stack windows on the right.


		// Move focused window to master area on the left.
		FLUKE_DEBUG_NOTICE_SUB("tile the master window.")
		fluke::configure_window(
			conn, focused, fluke::XCB_MOVE_RESIZE,

			display_x,
			display_y,
			std::ceil(display_w / 2.f) - (fluke::config::BORDER_SIZE * 2),
			display_h
		);


		// Stack up all of the slave windows on the right.
		FLUKE_DEBUG_NOTICE_SUB("tile the slave windows.")
		for (auto win: windows) {
			if (win == focused)
				continue;

			// Move window to empty space.
			fluke::configure_window(
				conn, win, fluke::XCB_MOVE_RESIZE,

				std::ceil(display_x + display_w / 2.f) + fluke::config::GAP,
				sliding_y,

				std::ceil(display_w / 2.f) - fluke::config::GAP,
				slave_height
			);

			// Increment Y position for next window.
			sliding_y += slave_height + (fluke::config::BORDER_SIZE * 2) + fluke::config::GAP;
		}

	}

	inline void action_layout_monocle(fluke::Connection&) {

	}
}

