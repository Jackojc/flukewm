#pragma once

#include <utility>
#include <algorithm>

#include <fluke.hpp>


namespace fluke {
	// Function signature for event handlers: void(*)(fluke::Connection&, fluke::Event&&);

	inline void event_enter_notify(fluke::Connection& conn, fluke::Event&& e_) {
		auto e = fluke::event_cast<fluke::EnterNotifyEvent>(std::move(e_));
		xcb_window_t win = e->event;

		if (e->mode != XCB_NOTIFY_MODE_NORMAL and e->mode != XCB_NOTIFY_MODE_UNGRAB)
			return;

		fluke::on_hover_in(conn);
		FLUKE_DEBUG_NOTICE(
			"event '", tinge::fg::make_yellow("ENTER_NOTIFY"),
			"' for '", tinge::fg::make_yellow(fluke::to_hex(win)), "'"
		)

		// set input focus to new window, set borders and stacking order.
		if constexpr(fluke::config::MOUSE_FOCUS)
			fluke::set_input_focus(conn, XCB_INPUT_FOCUS_PARENT, win);
	}



	inline void event_leave_notify(fluke::Connection& conn, fluke::Event&& e_) {
		auto e = fluke::event_cast<fluke::LeaveNotifyEvent>(std::move(e_));
		xcb_window_t win = e->event;

		fluke::on_hover_out(conn);
		FLUKE_DEBUG_NOTICE(
			"event '", tinge::fg::make_yellow("LEAVE_NOTIFY"),
			"' for '", tinge::fg::make_yellow(fluke::to_hex(win)), "'"
		)


		// enable this to unfocus window when pointer is moved outside of any windows
		// if constexpr(not config::USE_LAZY_FOCUS)
		// 	fluke::set_input_focus(conn, XCB_INPUT_FOCUS_PARENT, conn.root());
	}



	inline void event_focus_in(fluke::Connection& conn, fluke::Event&& e_) {
		auto e = fluke::event_cast<fluke::FocusInEvent>(std::move(e_));
		xcb_window_t win = e->event;

		if (
			e->mode == XCB_NOTIFY_MODE_GRAB or
			e->mode == XCB_NOTIFY_MODE_UNGRAB or
			e->detail == XCB_NOTIFY_DETAIL_POINTER or
			e->detail == XCB_NOTIFY_DETAIL_POINTER_ROOT or
			e->detail == XCB_NOTIFY_DETAIL_NONE
		) {
			return;
		}

		fluke::on_focus_in(conn);
		FLUKE_DEBUG_NOTICE(
			"event '", tinge::fg::make_yellow("FOCUS_IN"),
			"' for '", tinge::fg::make_yellow(fluke::to_hex(win)), "'"
		)

		// Move cursor to center of window.
		auto [x_, y_, w, h] = fluke::as_rect(fluke::get(conn, fluke::get_geometry(conn, win)));

		auto x = w / 2;
		auto y = h / 2;

		fluke::warp_pointer(conn, XCB_NONE, win, 0, 0, 0, 0, x, y);

		// fluke::configure_window(conn, win, XCB_CONFIG_WINDOW_STACK_MODE, XCB_STACK_MODE_ABOVE);
		fluke::change_window_attributes(conn, win, XCB_CW_BORDER_PIXEL, config::BORDER_COLOUR_ACTIVE);
	}



	inline void event_focus_out(fluke::Connection& conn, fluke::Event&& e_) {
		auto e = fluke::event_cast<fluke::FocusOutEvent>(std::move(e_));
		xcb_window_t win = e->event;

		// This is needed to prevent border from flickering when moving/resizing with keybinds.
		if (e->mode == XCB_NOTIFY_MODE_GRAB or e->mode == XCB_NOTIFY_MODE_UNGRAB) return;

		fluke::on_focus_out(conn);
		FLUKE_DEBUG_NOTICE(
			"event '", tinge::fg::make_yellow("FOCUS_OUT"),
			"' for '", tinge::fg::make_yellow(fluke::to_hex(win)), "'"
		)

		fluke::change_window_attributes(conn, win, XCB_CW_BORDER_PIXEL, config::BORDER_COLOUR_INACTIVE);
	}



	inline void event_create_notify(fluke::Connection& conn, fluke::Event&& e_) {
		auto e = fluke::event_cast<fluke::CreateNotifyEvent>(std::move(e_));
		xcb_window_t win = e->window;

		if (fluke::is_ignored(fluke::get(conn, fluke::get_window_attributes(conn, win))))
			return;

		fluke::on_create(conn);
		FLUKE_DEBUG_NOTICE(
			"event '", tinge::fg::make_yellow("CREATE_NOTIFY"),
			"' for '", tinge::fg::make_yellow(fluke::to_hex(win)), "'"
		)


		// Find the currently focused display to launch the new window on.
		xcb_window_t focused = fluke::get_focused_window(conn);
		auto focused_rect = fluke::as_rect(fluke::get(conn, fluke::get_geometry(conn, focused)));

		auto [window_x, window_y, window_w, window_h] = focused_rect;
		auto [display_x, display_y, display_w, display_h] = fluke::get_nearest_display_rect(conn, focused_rect);

		// Resize window to a percentage of the screen size.
		auto w = (display_w * fluke::config::NEW_WINDOW_PERCENT) / 100;
		auto h = (display_h * fluke::config::NEW_WINDOW_PERCENT) / 100;

		// Center the window on the screen.
		auto x = (display_x + display_w / 2) - (window_x + w / 2);
		auto y = (display_y + display_h / 2) - (window_y + h / 2);

		// Register to receive events from the window and resize/move the window.
		fluke::change_window_attributes(conn, win, XCB_CW_EVENT_MASK, fluke::XCB_WINDOW_EVENTS);
		fluke::configure_window(
			conn, win,
			fluke::XCB_MOVE_RESIZE | XCB_CONFIG_WINDOW_BORDER_WIDTH,
			x, y, w, h, config::BORDER_SIZE
		);
	}



	inline void event_destroy_notify(fluke::Connection& conn, fluke::Event&& e_) {
		auto e = fluke::event_cast<fluke::DestroyNotifyEvent>(std::move(e_));
		xcb_window_t win = e->window;

		fluke::on_destroy(conn);
		FLUKE_DEBUG_NOTICE(
			"event '", tinge::fg::make_yellow("DESTROY_NOTIFY"),
			"' for '", tinge::fg::make_yellow(fluke::to_hex(win)), "'"
		)

		// Focus next alive window
		auto windows = fluke::get_mapped_windows(conn);

		if (windows.size() == 0)
			return;

		fluke::configure_window(conn, windows.back(), XCB_CONFIG_WINDOW_STACK_MODE, XCB_STACK_MODE_ABOVE);
		fluke::set_input_focus(conn, XCB_INPUT_FOCUS_PARENT, windows.back());
	}



	inline void event_map_request(fluke::Connection& conn, fluke::Event&& e_) {
		auto e = fluke::event_cast<fluke::MapRequestEvent>(std::move(e_));
		xcb_window_t win = e->window;

		fluke::on_map(conn);
		FLUKE_DEBUG_NOTICE(
			"event '", tinge::fg::make_yellow("MAP_REQUEST"),
			"' for '", tinge::fg::make_yellow(fluke::to_hex(win)), "'"
		)

		fluke::map_window(conn, win);
		fluke::configure_window(conn, win, XCB_CONFIG_WINDOW_STACK_MODE, XCB_STACK_MODE_ABOVE);
		fluke::set_input_focus(conn, XCB_INPUT_FOCUS_PARENT, win);
	}



	inline void event_unmap_notify(fluke::Connection& conn, fluke::Event&& e_) {
		auto e = fluke::event_cast<fluke::UnmapNotifyEvent>(std::move(e_));
		xcb_window_t win = e->window;

		fluke::on_unmap(conn);
		FLUKE_DEBUG_NOTICE(
			"event '", tinge::fg::make_yellow("UNMAP_NOTIFY"),
			"' for '", tinge::fg::make_yellow(fluke::to_hex(win)), "'"
		)
	}



	// this event cant be very hot if using a tool like xmmv
	inline void event_configure_request(fluke::Connection& conn, fluke::Event&& e_) {
		auto e = fluke::event_cast<fluke::ConfigureRequestEvent>(std::move(e_));
		xcb_window_t win = e->window;
		uint16_t mask = e->value_mask;

		fluke::on_configure(conn);
		FLUKE_DEBUG_NOTICE(
			"event '", tinge::fg::make_yellow("CONFIGURE_REQUEST"),
			"' for '", tinge::fg::make_yellow(fluke::to_hex(win)), "'"
		)


		// this functions essentially as a stack to push values onto.
		uint8_t i = 0;
		std::array<uint32_t, 7> values{};

		if (mask & XCB_CONFIG_WINDOW_X)            values[i++] = static_cast<uint32_t>(e->x);
		if (mask & XCB_CONFIG_WINDOW_Y)            values[i++] = static_cast<uint32_t>(e->y);
		if (mask & XCB_CONFIG_WINDOW_WIDTH)        values[i++] = e->width;
		if (mask & XCB_CONFIG_WINDOW_HEIGHT)       values[i++] = e->height;
		if (mask & XCB_CONFIG_WINDOW_BORDER_WIDTH) values[i++] = e->border_width;
		if (mask & XCB_CONFIG_WINDOW_SIBLING)      values[i++] = e->sibling;
		if (mask & XCB_CONFIG_WINDOW_STACK_MODE)   values[i++] = e->stack_mode;

		fluke::configure_window(conn, win, mask, values.data());
	}



	inline void event_property_notify(fluke::Connection& conn, fluke::Event&& e_) {
		auto e = fluke::event_cast<fluke::PropertyNotifyEvent>(std::move(e_));

		fluke::on_property(conn);
		FLUKE_DEBUG_NOTICE( "event '", tinge::fg::make_yellow("PROPERTY_NOTIFY"), "'" )
	}



	inline void event_client_message(fluke::Connection& conn, fluke::Event&& e_) {
		auto e = fluke::event_cast<fluke::ClientMessageEvent>(std::move(e_));

		fluke::on_client_message(conn);
		FLUKE_DEBUG_NOTICE( "event '", tinge::fg::make_yellow("CLIENT_MESSAGE"), "'" )
	}



	inline void event_randr_screen_change_notify(fluke::Connection& conn, fluke::Event&& e_) {
		auto e = fluke::event_cast<fluke::RandrScreenChangeNotifyEvent>(std::move(e_));

		fluke::on_randr_screen_change(conn);
		FLUKE_DEBUG_NOTICE( "event '", tinge::fg::make_yellow("RANDR_SCREEN_CHANGE_NOTIFY"), "'" )

		// move windows that are off screen back into view.
	}



	inline void event_randr_notify(fluke::Connection& conn, fluke::Event&& e_) {
		auto e = fluke::event_cast<fluke::RandrNotifyEvent>(std::move(e_));

		fluke::on_randr_notify(conn);
		FLUKE_DEBUG_NOTICE( "event '", tinge::fg::make_yellow("RANDR_NOTIFY"), "'" )
	}



	inline void event_keypress(fluke::Connection& conn, fluke::Event&& e_) {
		auto e = fluke::event_cast<fluke::KeyPressEvent>(std::move(e_));
		xcb_keysym_t keysym = fluke::get_keysym(conn, e->detail);

		fluke::on_keypress(conn);
		FLUKE_DEBUG_NOTICE( "event '", tinge::fg::make_yellow("KEYPRESS"), "'" )


		constexpr auto clean = [] (unsigned mask) {
			return mask & ~(fluke::XCB_MASK_CAPS_LOCK | fluke::XCB_MASK_NUM_LOCK | fluke::XCB_MASK_SCROLL_LOCK);
		};


		// Find `fluke::Key` structure which has a matching keysym and modifier.
		for (auto& [mod, sym, func]: fluke::config::keys) {
			if (keysym == sym and clean(mod) == clean(e->state)) {
				func(conn);
				return;
			}
		}
	}



	inline void event_error(fluke::Connection& conn, fluke::Event&& e_) {
		auto e = fluke::event_cast<fluke::Error>(std::move(e_));

		int major_code = e->major_code;
		int minor_code = e->minor_code;
		int error_code = e->error_code;

		// Error code `3` is a bad window error, these errors are pretty much
		// guaranteed to happen and are normal so we wont print them.
		if (error_code == 3)
			return;

		fluke::on_error(conn);
		FLUKE_DEBUG_NOTICE( "event '", tinge::fg::make_yellow("ERROR"), "'" )

		// Make error names bright blue.
		auto major = tinge::fg::bright::make_blue(fluke::request_str[e->major_code]);
		auto error = tinge::fg::bright::make_blue(fluke::error_str[e->error_code]);
		auto help  = fluke::help_str[e->error_code];

		// Make error codes dim blue.
		auto major_code_str = tinge::fg::dim::make_blue(e->major_code);
		auto minor_code_str = tinge::fg::dim::make_blue(e->minor_code);
		auto error_code_str = tinge::fg::dim::make_blue(e->error_code);

		// Pretty string with all error codes concatenated.
		auto all_codes_str = tinge::strcat(
			"major(", major_code_str,
			"), minor(", minor_code_str,
			"), error(", error_code_str, ")"
		);

		// Print the error with some formatting and colours. A bit messy...
		tinge::errorln(
			"request '", major, "' failed with '", error, "'!",
			"\n\t", tinge::fg::dim::make_cyan("code  "), all_codes_str,
			"\n\t", tinge::fg::dim::make_cyan("help  "), help
		);
	}
}

