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


		xcb_window_t focused = fluke::get_focused_window(conn);

		auto [x, y, w, h] = fluke::as_rect(fluke::get(conn, fluke::get_geometry(conn, focused)));

		auto new_x = static_cast<uint32_t>(x + x_amount);
		auto new_y = static_cast<uint32_t>(y + y_amount);
		auto new_w = static_cast<uint32_t>(w + w_amount);
		auto new_h = static_cast<uint32_t>(h + h_amount);

		fluke::configure_window(conn, focused, fluke::XCB_MOVE_RESIZE, new_x, new_y, new_w, new_h);
	}


	// Change workspace
	inline void action_workspace_change(fluke::Connection& conn, const std::vector<int>& arg) {

	}

	inline void action_workspace_send_to(fluke::Connection& conn, const std::vector<int>& arg) {

	}





	// scratchpad
	inline void action_scratchpad_show(fluke::Connection& conn, const std::vector<int>& arg) {

	}

	inline void action_scratchpad_hide(fluke::Connection& conn, const std::vector<int>& arg) {

	}

	inline void action_scratchpad_toggle(fluke::Connection& conn, const std::vector<int>& arg) {

	}

	inline void action_scratchpad_add(fluke::Connection& conn, const std::vector<int>& arg) {

	}

	inline void action_scratchpad_remove(fluke::Connection& conn, const std::vector<int>& arg) {

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
		xcb_window_t focused = fluke::get_focused_window(conn);

		// Vector of all windows currently mapped.
		auto windows = fluke::get_mapped_windows(conn);

		if (windows.size() == 0)
			return;

		// Get the geometry of all mapped windows.
		// We also want to find the index for the geometry of the focused
		// specifically too.
		decltype(windows)::size_type i = 0;
		auto geoms = fluke::dispatch_consume(conn, [&] (xcb_window_t win) {
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
		auto [fx, fy, fw, fh] = fluke::as_rect(geoms.at(i));

		// Get the midpoint of the relevant side of the currently focused window.
		const std::array fsides{
			left_side(fx, fy, fw, fh),
			right_side(fx, fy, fw, fh),
			top_side(fx, fy, fw, fh),
			bottom_side(fx, fy, fw, fh),
		};

		fluke::Point fpoint = fsides.at(static_cast<decltype(fsides)::size_type>(dir));


		// Store distances to windows and the associated window IDs.
		std::vector<std::pair<xcb_window_t, int>> distances;

		// Get the distance from center point of focused window to another point.
		const auto distance = [&fpoint] (const fluke::Point& p) {
			return std::sqrt(std::pow(fpoint.x - p.x, 2) + std::pow(fpoint.y - p.y, 2));
		};


		// Loop over all windows and get the midpoint of all of their four sides.
		// We get the point which is nearest to the focused window and
		// add it to our `distances` vector.
		for (auto [win, geom]: fluke::zip(windows, geoms)) {
			if (win == focused)
				continue;

			auto [x, y, w, h] = fluke::as_rect(geom);

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
		auto nearest_win = std::min_element(distances.begin(), distances.end(), [] (const auto& a, const auto& b) {
			return a.second < b.second;
		})->first;

		// Set input focus to new window.
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
		auto windows = fluke::get_mapped_windows(conn);

		if (windows.size() == 0)
			return;

		// Get the currently focused window.
		xcb_window_t focused = fluke::get_focused_window(conn);


		// Depending on which direction we are focusing, we need to use
		// different options and windows.
		std::array opts {
			std::pair{windows.back(), XCB_STACK_MODE_BELOW},  // Previous
			std::pair{windows.front(), XCB_STACK_MODE_ABOVE}, // Next
		};

		auto[next_win, stack_mode] = opts.at(static_cast<decltype(opts)::size_type>(dir));

		// Set focus to new window and shuffle the window stack around.
		fluke::configure_window(conn, focused, XCB_CONFIG_WINDOW_STACK_MODE, stack_mode);
		fluke::configure_window(conn, next_win, XCB_CONFIG_WINDOW_STACK_MODE, stack_mode);
		fluke::set_input_focus(conn, XCB_INPUT_FOCUS_PARENT, next_win);
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
		auto windows = fluke::get_mapped_windows(conn);

		if (windows.size() == 0)
			return;


		// Get focused window ID and its rect.
		xcb_window_t focused = fluke::get_focused_window(conn);
		auto focused_rect = fluke::as_rect(fluke::get(conn, fluke::get_geometry(conn, focused)));

		// Get the rect of the nearest display to the focused window.
		auto [display_x, display_y, display_w, display_h] = fluke::get_nearest_display_rect(conn, focused_rect);


		// Get the height that each slave window should be.
		int slave_height = display_h / (windows.size() - 1) - (fluke::config::BORDER_SIZE * 2);
		int sliding_y = display_y;  // Keep track of Y position so we can stack windows on the right.

		// Get the usable display area.
		// Leave a gap for window borders.
		display_w -= fluke::config::BORDER_SIZE * 2;
		display_h -= fluke::config::BORDER_SIZE * 2;


		// Move focused window to master area on the left.
		fluke::configure_window(
			conn, focused, fluke::XCB_MOVE_RESIZE,

			display_x,
			display_y,
			display_w / 2 - (fluke::config::BORDER_SIZE * 2),
			display_h
		);


		// Stack up all of the slave windows on the right.
		for (auto win: windows) {
			if (win == focused)
				continue;

			// Move window to empty space.
			fluke::configure_window(
				conn, win, fluke::XCB_MOVE_RESIZE,

				display_x + display_w / 2,
				sliding_y,
				display_w / 2,
				slave_height
			);

			// Increment Y position for next window.
			sliding_y += slave_height + (fluke::config::BORDER_SIZE * 2);
		}

	}

	inline void action_layout_monocle(fluke::Connection& conn) {

	}


}

