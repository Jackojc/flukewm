#pragma once

#include <algorithm>
#include <fluke.hpp>

namespace fluke {
	// Move, Resize, Grow, Shrink
	void action_resize(fluke::Connection& conn, const std::vector<int>& args) {
		// args: x, y, width, height
		auto x_amount = args.at(0);
		auto y_amount = args.at(1);
		auto w_amount = args.at(2);
		auto h_amount = args.at(3);

		xcb_window_t focused = fluke::get(conn, fluke::get_input_focus(conn))->focus;
		auto geom = fluke::get(conn, fluke::get_geometry(conn, focused));

		auto x = geom->x;
		auto y = geom->y;
		auto w = geom->width;
		auto h = geom->height;

		auto new_x = static_cast<uint32_t>(x + x_amount);
		auto new_y = static_cast<uint32_t>(y + y_amount);
		auto new_w = static_cast<uint32_t>(w + w_amount);
		auto new_h = static_cast<uint32_t>(h + h_amount);

		fluke::configure_window(
			conn,
			focused,
			XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y | XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT,
			new_x, new_y, new_w, new_h
		);
	}


	// Change workspace
	void action_workspace_change(fluke::Connection& conn, const std::vector<int>& arg) {

	}

	void action_workspace_send_to(fluke::Connection& conn, const std::vector<int>& arg) {

	}





	// scratchpad
	void action_scratchpad_show(fluke::Connection& conn, const std::vector<int>& arg) {

	}

	void action_scratchpad_hide(fluke::Connection& conn, const std::vector<int>& arg) {

	}

	void action_scratchpad_toggle(fluke::Connection& conn, const std::vector<int>& arg) {

	}

	void action_scratchpad_add(fluke::Connection& conn, const std::vector<int>& arg) {

	}

	void action_scratchpad_remove(fluke::Connection& conn, const std::vector<int>& arg) {

	}







	// Alt tab like focusing
	void action_focus_dir(fluke::Connection& conn, const std::vector<int>& arg) {

	}


	enum {
		FOCUS_PREV = -1,
		FOCUS_NEXT = +1,
	};

	void action_focus(fluke::Connection& conn, const std::vector<int>& args) {
		auto dir = args.at(0);

		// Get focused window aswell as all mapped windows.
		xcb_window_t focused = fluke::get(conn, fluke::get_input_focus(conn))->focus;
		auto windows = fluke::get_mapped_windows(conn);

		// Find index of focused window in windows vector.
		int i = 0;
		for (; windows.at(i) != focused; i++) {}

		// Get the next window in the list (using modulus to make it circular)
		xcb_window_t next_win = windows.at((i + dir) % windows.size());

		// Set border colours.
		fluke::change_window_attributes(conn, next_win, XCB_CW_BORDER_PIXEL, fluke::config::BORDER_COLOUR_ACTIVE);
		fluke::change_window_attributes(conn, focused, XCB_CW_BORDER_PIXEL, fluke::config::BORDER_COLOUR_INACTIVE);

		// Rotate stack of windows by 1.
		auto rotated_win = (dir == FOCUS_NEXT) ? windows.back() : windows.front();
		fluke::configure_window(conn, rotated_win, XCB_CONFIG_WINDOW_STACK_MODE, XCB_STACK_MODE_ABOVE);

		// Put focused window on top of the stack.
		fluke::configure_window(conn, next_win, XCB_CONFIG_WINDOW_STACK_MODE, XCB_STACK_MODE_ABOVE);

		// Set input focus to new window.
		fluke::set_input_focus(conn, XCB_INPUT_FOCUS_POINTER_ROOT, next_win);
	}

	// void action_focus_prev(fluke::Connection& conn, const std::vector<int>&) {
	// 	xcb_window_t focused = fluke::get(conn, fluke::get_input_focus(conn))->focus;
	// 	auto windows = fluke::get_mapped_windows(conn);

	// 	int i = 0;
	// 	for (; windows.at(i) != focused; i++) {}

	// 	xcb_window_t next_win = windows.at((i - 1) % windows.size());

	// 	fluke::change_window_attributes(conn, next_win, XCB_CW_BORDER_PIXEL, fluke::config::BORDER_COLOUR_ACTIVE);
	// 	fluke::change_window_attributes(conn, focused, XCB_CW_BORDER_PIXEL, fluke::config::BORDER_COLOUR_INACTIVE);

	// 	fluke::configure_window(conn, windows.front(), XCB_CONFIG_WINDOW_STACK_MODE, XCB_STACK_MODE_ABOVE);
	// 	fluke::configure_window(conn, next_win, XCB_CONFIG_WINDOW_STACK_MODE, XCB_STACK_MODE_ABOVE);

	// 	fluke::set_input_focus(conn, XCB_INPUT_FOCUS_POINTER_ROOT, next_win);
	// }








	// Tiling
	void action_layout_masterslave(fluke::Connection& conn, const std::vector<int>& arg) {

	}

	void action_layout_monocle(fluke::Connection& conn, const std::vector<int>& arg) {

	}


}

