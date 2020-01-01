#pragma once

#include <utility>
#include <algorithm>

#include <fluke.hpp>


namespace fluke::event_handlers {
	// void(*)(fluke::Connection&, fluke::Event&&);

	inline void print_event_name(const char* name, xcb_window_t win) {
		FLUKE_DEBUG( tinge::noticeln("event '", tinge::fg::yellow, name, tinge::reset, "' for '", tinge::fg::yellow , fluke::to_hex(win), tinge::reset, "'") )
	}


	inline void event_enter_notify(fluke::Connection& conn, fluke::Event&& e_) {
		auto e = fluke::event_cast<fluke::EnterNotifyEvent>(std::move(e_));
		xcb_window_t win = e->event;

		print_event_name("ENTER_NOTIFY", win);

		if (e->mode != XCB_NOTIFY_MODE_NORMAL and e->mode != XCB_NOTIFY_MODE_UNGRAB)
			return;

		// set input focus to new window, set borders and stacking order.
		fluke::set_input_focus(conn, XCB_INPUT_FOCUS_POINTER_ROOT, win);
		fluke::configure_window(conn, win, XCB_CONFIG_WINDOW_STACK_MODE, XCB_STACK_MODE_ABOVE);
	}



	inline void event_leave_notify(fluke::Connection& conn, fluke::Event&& e_) {
		auto e = fluke::event_cast<fluke::LeaveNotifyEvent>(std::move(e_));
		xcb_window_t win = e->event;

		print_event_name("LEAVE_NOTIFY", win);

		// enable this to unfocus window when pointer is moved outside of any windows
		// fluke::SetInputFocus{conn, XCB_INPUT_FOCUS_POINTER_ROOT, conn.root(), XCB_CURRENT_TIME};
	}



	inline void event_focus_in(fluke::Connection& conn, fluke::Event&& e_) {
		auto e = fluke::event_cast<fluke::FocusInEvent>(std::move(e_));
		xcb_window_t win = e->event;

		print_event_name("FOCUS_IN", win);

		fluke::change_window_attributes(conn, win, XCB_CW_BORDER_PIXEL, constants::BORDER_COLOUR_ACTIVE);
	}



	inline void event_focus_out(fluke::Connection& conn, fluke::Event&& e_) {
		auto e = fluke::event_cast<fluke::FocusOutEvent>(std::move(e_));
		xcb_window_t win = e->event;

		print_event_name("FOCUS_OUT", win);

		fluke::change_window_attributes(conn, win, XCB_CW_BORDER_PIXEL, constants::BORDER_COLOUR_INACTIVE);
	}





	inline void event_create_notify(fluke::Connection& conn, fluke::Event&& e_) {
		auto e = fluke::event_cast<fluke::CreateNotifyEvent>(std::move(e_));
		xcb_window_t win = e->window;

		print_event_name("CREATE_NOTIFY", win);

		if (fluke::get(conn, fluke::get_window_attributes(conn, win))->override_redirect)
			return;

		auto&& [cursor, geom] = fluke::get(conn,
			fluke::query_pointer(conn, conn.root()),
			fluke::get_geometry(conn, win)
		);

		auto cursor_x = cursor->root_x;
		auto cursor_y = cursor->root_y;
		auto w = geom->width;
		auto h = geom->height;
		auto bw = geom->border_width;

		// todo: make this work based on currently focused monitor
		auto screen_w = conn.screen()->width_in_pixels;
		auto screen_h = conn.screen()->height_in_pixels;

		auto x = static_cast<uint32_t>(std::clamp(cursor_x - w / 2, 0, screen_w - w - bw));
		auto y = static_cast<uint32_t>(std::clamp(cursor_y - h / 2, 0, screen_h - h - bw));

		fluke::change_window_attributes(conn, win, XCB_CW_EVENT_MASK, fluke::XCB_WINDOW_EVENTS);
		fluke::configure_window(conn, win, fluke::XCB_MOVE_RESIZE, x, y, w, h);
	}



	inline void event_map_request(fluke::Connection& conn, fluke::Event&& e_) {
		auto e = fluke::event_cast<fluke::MapRequestEvent>(std::move(e_));
		xcb_window_t win = e->window;

		print_event_name("MAP_REQUEST", win);

		if (fluke::get(conn, fluke::get_window_attributes(conn, win))->override_redirect)
			return;

		fluke::map_window(conn, win);
		fluke::change_window_attributes(conn, win, XCB_CW_BORDER_PIXEL, constants::BORDER_COLOUR_ACTIVE);
		fluke::configure_window(conn, win, XCB_CONFIG_WINDOW_BORDER_WIDTH, constants::BORDER_SIZE);
	}


	// this event cant be very hot if using a tool like xmmv
	inline void event_configure_request(fluke::Connection& conn, fluke::Event&& e_) {
		auto e = fluke::event_cast<fluke::ConfigureRequestEvent>(std::move(e_));
		xcb_window_t win = e->window;
		uint16_t mask = e->value_mask;

		print_event_name("CONFIGURE_REQUEST", win);

		// this functions essentially as a stack to push values onto.
		uint8_t i = 0;
		std::array<uint32_t, 5> values{};

		if (mask & XCB_CONFIG_WINDOW_X)          values[i++] = static_cast<uint32_t>(e->x);
		if (mask & XCB_CONFIG_WINDOW_Y)          values[i++] = static_cast<uint32_t>(e->y);
		if (mask & XCB_CONFIG_WINDOW_WIDTH)      values[i++] = e->width;
		if (mask & XCB_CONFIG_WINDOW_HEIGHT)     values[i++] = e->height;
		if (mask & XCB_CONFIG_WINDOW_STACK_MODE) values[i++] = e->stack_mode;

		fluke::configure_window(conn, win, mask, values.data());
	}


	inline void event_error(fluke::Connection& conn, fluke::Event&& e_) {
		auto e = fluke::event_cast<fluke::Error>(std::move(e_));

		auto major = fluke::major_str[e->major_code];
		auto error = fluke::error_str[e->error_code];
		auto help = fluke::help_str[e->error_code];

		int major_code = e->major_code;
		int minor_code = e->minor_code;
		int error_code = e->error_code;

		// im sorry
		tinge::errorln(
			"request '", tinge::fg::bright::blue, major, tinge::reset,
			"' failed with '", tinge::fg::blue, error, tinge::reset, "'!",
			"\n\t", tinge::fg::dim::cyan, "code  ", tinge::reset, "major(", tinge::fg::dim::blue, major_code, tinge::reset, "), minor(", tinge::fg::dim::blue, minor_code, tinge::reset, "), error(", tinge::fg::dim::blue, error_code, tinge::reset, ")",
			"\n\t", tinge::fg::dim::cyan, "help  ", tinge::reset, help
		);
	}

}

