#ifndef FLUKE_ACTIONS_HPP
#define FLUKE_ACTIONS_HPP

#pragma once

#include <algorithm>
#include <type_traits>
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
		const xcb_window_t focused = fluke::get_focused_window(conn);

		if (not fluke::is_valid_window(conn, focused))
			return;

		// Get geometry of focused window and calculate offsets for the new window rect.
		auto [x, y, w, h] = fluke::as_rect(fluke::get(conn, fluke::get_geometry(conn, focused)));

		x += x_amount;
		y += y_amount;
		w += w_amount;
		h += h_amount;

		fluke::configure_window(conn, focused, fluke::XCB_MOVE_RESIZE, x, y, w, h);
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







	inline void action_focus_display_index(fluke::Connection& conn, int index) {
		FLUKE_DEBUG_NOTICE(
			"action '", tinge::fg::make_yellow("FOCUS_DISPLAY_INDEX"),
			"' with arg(s) '", tinge::fg::make_yellow(index), "'"
		)

		auto displays = fluke::get_crtcs(conn);

		if (std::make_unsigned_t<int>(index) >= displays.size())
			return;

		fluke::center_pointer_in_rect(conn, fluke::as_rect(displays.at(std::make_unsigned_t<int>(index))));
	}








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
		const xcb_window_t focused = fluke::get_focused_window(conn);

		if (not fluke::is_valid_window(conn, focused))
			return;

		// Vector of all windows currently mapped.
		const auto windows = fluke::get_mapped_windows(conn);

		if (windows.size() <= 1)
			return;

		// Get the geometry of all mapped windows.
		// We also want to find the index for the geometry of the focused
		// specifically too.
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
		const auto focused_rect = fluke::as_rect(geoms.at(i));
		const auto [fx, fy, fw, fh] = focused_rect;


		// Get the midpoint of the relevant side of the currently focused window.
		const auto fpoint = std::array{
			left_side(fx, fy, fw, fh),
			right_side(fx, fy, fw, fh),
			top_side(fx, fy, fw, fh),
			bottom_side(fx, fy, fw, fh),
		}.at(std::make_unsigned_t<int>(dir));


		// Store distances to windows and the associated window IDs.
		std::vector<std::pair<xcb_window_t, int>> distances;
		distances.reserve(windows.size());


		// Loop over all windows and get the midpoint of all of their four sides.
		// We get the point which is nearest to the focused window and
		// add it to our `distances` vector.
		for (const auto [win, geom]: fluke::zip(windows, geoms)) {
			if (win == focused)
				continue;

			const auto point = fluke::get_rect_center(fluke::as_rect(geom));


			// Check if the window we are currently checking is present in the direction
			// we wish to focus.
			// For example: if we are focusing to the right, we only consider windows
			// which are to the right of the focused window.
			if (not std::array{
				point.x < fpoint.x,  // Left.
				point.x > fpoint.x,  // Right.
				point.y < fpoint.y,  // Top.
				point.y > fpoint.y,  // Bottom.
			}.at(std::make_unsigned_t<int>(dir))) {
				continue;
			}

			// If the window is valid, calculate the distance between the reference
			// point on the focused window to the point on the appropriate side of
			// the window we are checking. Add it to the vector of distances.
			distances.emplace_back(win, fluke::distance_abs(fpoint, point));
		}

		// Find window with nearest distance.
		const auto nearest_win = std::min_element(
			distances.begin(), distances.end(),

			[] (const auto& a, const auto& b) {
				return a.second < b.second;
			}
		)->first;

		// Set input focus to new window.
		fluke::configure_window(conn, nearest_win, XCB_CONFIG_WINDOW_STACK_MODE, XCB_STACK_MODE_ABOVE);
		fluke::set_input_focus(conn, XCB_INPUT_FOCUS_PARENT, nearest_win);
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
		auto windows = fluke::get_mapped_windows_on_hovered_display(conn);

		if (windows.size() <= 1)
			return;

		// Get the currently focused window.
		const xcb_window_t focused = fluke::get_focused_window(conn);

		fluke::remove_focused_window(conn, windows);

		// Depending on which direction we are focusing, we need to use
		// different options and windows.
		const auto [next_win, stack_mode] = std::array{
			std::pair{windows.back(), XCB_STACK_MODE_ABOVE},  // Previous
			std::pair{windows.front(), XCB_STACK_MODE_BELOW}, // Next
		}.at(std::make_unsigned_t<int>(dir));

		// Set focus to new window and shuffle the window stack around.
		fluke::configure_window(conn, focused, XCB_CONFIG_WINDOW_STACK_MODE, stack_mode);
		fluke::configure_window(conn, next_win, XCB_CONFIG_WINDOW_STACK_MODE, XCB_STACK_MODE_ABOVE);
		fluke::set_input_focus(conn, XCB_INPUT_FOCUS_PARENT, next_win);
	}








	// Misc actions
	inline void action_center(fluke::Connection& conn) {
		FLUKE_DEBUG_NOTICE("action '", tinge::fg::make_yellow("CENTER"), "'")

		// Get the focused window and its geometry.
		const xcb_window_t focused = fluke::get_focused_window(conn);

		if (not fluke::is_valid_window(conn, focused))
			return;

		fluke::center_window_on_hovered_display(conn, focused);
	}



	inline void action_center_resize(fluke::Connection& conn) {
		FLUKE_DEBUG_NOTICE("action '", tinge::fg::make_yellow("CENTER_RESIZE"), "'")

		// Get the focused window and its geometry.
		const xcb_window_t focused = fluke::get_focused_window(conn);

		if (not fluke::is_valid_window(conn, focused))
			return;

		// Find the currently focused display to launch the new window on.
		fluke::center_resize_window_on_hovered_display(conn, focused);
	}









	// Window snapping
	enum {
		SNAP_SIDE_LEFT,
		SNAP_SIDE_RIGHT,
		SNAP_SIDE_TOP,
		SNAP_SIDE_BOTTOM,

		SNAP_CORNER_TOPLEFT,
		SNAP_CORNER_TOPRIGHT,
		SNAP_CORNER_BOTTOMRIGHT,
		SNAP_CORNER_BOTTOMLEFT,
	};

	constexpr const char* side_str[] = {
		"SNAP_SIDE_LEFT",
		"SNAP_SIDE_RIGHT",
		"SNAP_SIDE_TOP",
		"SNAP_SIDE_BOTTOM",

		"SNAP_CORNER_TOPLEFT",
		"SNAP_CORNER_TOPRIGHT",
		"SNAP_CORNER_BOTTOMRIGHT",
		"SNAP_CORNER_BOTTOMLEFT",
	};

	inline void action_snap(fluke::Connection& conn, int side) {
		namespace conf = fluke::config;

		FLUKE_DEBUG_NOTICE(
			"action '", tinge::fg::make_yellow("SNAP"),
			"' with arg(s) '", tinge::fg::make_yellow(side_str[side]), "'"
		)

		// Get focused window.
		const xcb_window_t focused = fluke::get_focused_window(conn);

		if (not fluke::is_valid_window(conn, focused))
			return;

		const auto focused_rect = fluke::as_rect(fluke::get(conn, fluke::get_geometry(conn, focused)));

		// Get usable screen area.
		const auto [display_x, display_y, display_w, display_h] =
			fluke::get_adjusted_display_rect(fluke::get_nearest_display_rect(conn, focused_rect));


		// Get the rect of the side we wish to move our window into.
		const auto [x, y, w, h] = fluke::get_adjusted_window_rect( std::array{
			// Left side.
			fluke::Rect{
				display_x,
				display_y,
				display_w / 2,
				display_h
			},

			// Right side.
			fluke::Rect{
				display_x + display_w / 2,
				display_y,
				display_w / 2,
				display_h
			},

			// Top side.
			fluke::Rect{
				display_x,
				display_y,
				display_w,
				display_h / 2
			},

			// Bottom side.
			fluke::Rect{
				display_x,
				display_y + display_h / 2,
				display_w,
				display_h / 2
			},

			// Top left corner.
			fluke::Rect{
				display_x,
				display_y,
				display_w / 2,
				display_h / 2
			},

			// Top right corner.
			fluke::Rect{
				display_x + display_w / 2,
				display_y,
				display_w / 2,
				display_h / 2
			},

			// Bottom right corner.
			fluke::Rect{
				display_x + display_w / 2,
				display_y + display_h / 2,
				display_w / 2,
				display_h / 2
			},

			// Bottom left corner.
			fluke::Rect{
				display_x,
				display_y + display_h / 2,
				display_w / 2,
				display_h / 2
			}
		}.at(std::make_unsigned_t<int>(side)) );

		fluke::configure_window(conn, focused, fluke::XCB_MOVE_RESIZE, x, y, w, h);
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

	inline void action_layout_masterslave(fluke::Connection& conn, int master_side, int master_size) {
		FLUKE_DEBUG_NOTICE(
			"action '", tinge::fg::make_yellow("LAYOUT_MASTERSLAVE"),
			"' with arg(s) '", tinge::fg::make_yellow(master_str[master_side]), "'"
		)

		auto windows = fluke::get_mapped_windows_on_hovered_display(conn);
		if (windows.size() <= 1)
			return;

		// Get the rect of the display which contains the pointer and
		// the usable display area.
		const auto [display_x, display_y, display_w, display_h] =
			fluke::get_adjusted_display_rect(fluke::get_hovered_display_rect(conn));


		// Get widths of master and slave windows.
		const auto master_w = (display_w * master_size) / 100;
		const auto slave_w = display_w - master_w;


		// Get the height that each slave window should be.
		const float slave_h = float(display_h) / (windows.size() - 1);
		float sliding_y = display_y;  // Keep track of Y position so we can stack slave windows.


		for (auto win: windows) {
			fluke::Rect new_rect;

			// Master window.
			if (win == fluke::get_focused_window(conn)) {
				// Rectangle for master window.
				new_rect = fluke::get_adjusted_window_rect( std::array{
					fluke::Rect{ display_x, display_y, master_w, display_h },  // Left
					fluke::Rect{ display_x + slave_w, display_y, master_w, display_h },  // Right
				}.at(std::make_unsigned_t<int>(master_side)) );
			}

			// Slave windows.
			else {
				// Rectangle for slave windows, opposite to `master_side`.
				new_rect = fluke::get_adjusted_window_rect( std::array{
					fluke::Rect{ display_x + master_w, std::ceil(sliding_y), slave_w, slave_h },  // Right
					fluke::Rect{ display_x, std::ceil(sliding_y), slave_w, slave_h },  // Left
				}.at(std::make_unsigned_t<int>(master_side)) );

				// Increment Y position for next window.
				sliding_y += slave_h;
			}


			// Resize and move current window.
			const auto [x, y, w, h] = new_rect;
			fluke::configure_window(conn, win, fluke::XCB_MOVE_RESIZE, x, y, w, h);
		}
	}



	inline void action_layout_monocle(fluke::Connection& conn) {
		FLUKE_DEBUG_NOTICE("action '", tinge::fg::make_yellow("LAYOUT_MONOCLE"), "'")

		// Get all of the mapped windows.
		auto windows = fluke::get_mapped_windows_on_hovered_display(conn);

		if (windows.size() == 0)
			return;

		// Get the geometry for a fullscreen window on the current display.
		const auto [x, y, w, h] =
			fluke::get_adjusted_window_rect(
				fluke::get_adjusted_display_rect(fluke::get_hovered_display_rect(conn))
			);

		// Resize all windows on this display.
		for (auto win: windows)
			fluke::configure_window(conn, win, fluke::XCB_MOVE_RESIZE, x, y, w, h);
	}



	enum {
		STACK_VERTICAL,
		STACK_HORIZONTAL,
	};

	constexpr const char* stacked_str[] = {
		"STACK_VERTICAL",
		"STACK_HORIZONTAL",
	};

	inline void action_layout_stacked(fluke::Connection& conn, int stack_dir) {
		FLUKE_DEBUG_NOTICE(
			"action '", tinge::fg::make_yellow("LAYOUT_STACKED"),
			"' with arg(s) '", tinge::fg::make_yellow(stacked_str[stack_dir]), "'"
		)

		auto windows = fluke::get_mapped_windows_on_hovered_display(conn);

		if (windows.size() <= 1)
			return;



		const auto [display_x, display_y, display_w, display_h] =
			fluke::get_adjusted_display_rect(fluke::get_hovered_display_rect(conn));




		float winsize = 0;
		float sliding = 0;


		if (stack_dir == STACK_VERTICAL) {
			sliding = display_y;
			winsize = float(display_h) / (windows.size());
		}

		else if (stack_dir == STACK_HORIZONTAL) {
			sliding = display_x;
			winsize = float(display_w) / (windows.size());
		}




		// Resize all windows on this display.
		for (auto win: windows) {
			const auto [x, y, w, h] = fluke::get_adjusted_window_rect( std::array{
				fluke::Rect{ display_x, sliding, display_w, winsize },  // Vertical
				fluke::Rect{ sliding, display_y, winsize, display_h },  // Horizontal
			}.at(std::make_unsigned_t<int>(stack_dir)) );

			fluke::configure_window(conn, win, fluke::XCB_MOVE_RESIZE, x, y, w, h);
			sliding += winsize;
		}
	}





	inline void action_fullscreen(fluke::Connection& conn) {
		FLUKE_DEBUG_NOTICE("action '", tinge::fg::make_yellow("LAYOUT_MONOCLE"), "'")

		// Get focused window ID.
		const xcb_window_t focused = fluke::get_focused_window(conn);
		if (not fluke::is_valid_window(conn, focused))
			return;

		// Get the geometry for a fullscreen window on the current display.
		const auto [x, y, w, h] =
			fluke::get_adjusted_window_rect(
				fluke::get_adjusted_display_rect(fluke::get_hovered_display_rect(conn))
			);

		fluke::configure_window(conn, focused, fluke::XCB_MOVE_RESIZE, x, y, w, h);
	}


	inline void action_layout_grid(fluke::Connection&) {

	}
}

#endif
