#pragma once

#include <algorithm>
#include <fluke.hpp>


namespace fluke {
	/*
		This event is triggered whenever the pointer enters a window.
	*/
	inline void event_enter_notify(fluke::Connection& conn, const fluke::EnterNotifyEvent& e) {
		const xcb_window_t win = e->event;

		if (e->mode != XCB_NOTIFY_MODE_NORMAL and e->mode != XCB_NOTIFY_MODE_UNGRAB)
			return;

		fluke::on_hover_in(conn, e);
		FLUKE_DEBUG_NOTICE(
			"event '", tinge::fg::make_yellow("ENTER_NOTIFY"),
			"' for '", tinge::fg::make_yellow(fluke::to_hex(win)), "'"
		)
	}



	/*
		This event is triggered whenever the pointer leaves a window.
	*/
	inline void event_leave_notify(fluke::Connection& conn, const fluke::LeaveNotifyEvent& e) {
		const xcb_window_t win = e->event;

		fluke::on_hover_out(conn, e);
		FLUKE_DEBUG_NOTICE(
			"event '", tinge::fg::make_yellow("LEAVE_NOTIFY"),
			"' for '", tinge::fg::make_yellow(fluke::to_hex(win)), "'"
		)
	}



	/*
		This event is triggered when a window gains focus.

		We warp the pointer to the center of the focused window too.

		We also set the border colour to active.
	*/
	inline void event_focus_in(fluke::Connection& conn, const fluke::FocusInEvent& e) {
		const xcb_window_t win = e->event;

		if (
			e->mode == XCB_NOTIFY_MODE_GRAB or
			e->mode == XCB_NOTIFY_MODE_UNGRAB or
			e->detail == XCB_NOTIFY_DETAIL_POINTER or
			e->detail == XCB_NOTIFY_DETAIL_POINTER_ROOT or
			e->detail == XCB_NOTIFY_DETAIL_NONE
		) {
			return;
		}

		fluke::on_focus_in(conn, e);
		FLUKE_DEBUG_NOTICE(
			"event '", tinge::fg::make_yellow("FOCUS_IN"),
			"' for '", tinge::fg::make_yellow(fluke::to_hex(win)), "'"
		)

		// Move cursor to center of window.
		FLUKE_DEBUG_NOTICE_SUB("centering pointer inside window.")

		fluke::lock_pointer_to_window(conn, win);
		fluke::center_pointer_in_rect(conn, fluke::as_rect(fluke::get(conn, fluke::get_geometry(conn, win))));
		fluke::change_window_attributes(conn, win, XCB_CW_BORDER_PIXEL, config::BORDER_COLOUR_ACTIVE);
	}



	/*
		This event is triggered when a window loses focus.

		We set the border colour to inactive.
	*/
	inline void event_focus_out(fluke::Connection& conn, const fluke::FocusOutEvent& e) {
		const xcb_window_t win = e->event;

		// This is needed to prevent border from flickering when moving/resizing with keybinds.
		if (
			e->mode == XCB_NOTIFY_MODE_GRAB or
			e->mode == XCB_NOTIFY_MODE_UNGRAB
		) {
			return;
		}

		fluke::on_focus_out(conn, e);
		FLUKE_DEBUG_NOTICE(
			"event '", tinge::fg::make_yellow("FOCUS_OUT"),
			"' for '", tinge::fg::make_yellow(fluke::to_hex(win)), "'"
		)

		fluke::change_window_attributes(conn, win, XCB_CW_BORDER_PIXEL, config::BORDER_COLOUR_INACTIVE);
	}



	/*
		This event is triggered when a new window is created.

		We center the window and grow it to a proportion of the display size.

		We register to receive events from the window and set its border size.
	*/
	inline void event_create_notify(fluke::Connection& conn, const fluke::CreateNotifyEvent& e) {
		const xcb_window_t win = e->window;

		if (fluke::is_ignored(fluke::get(conn, fluke::get_window_attributes(conn, win))))
			return;

		fluke::on_create(conn, e);
		FLUKE_DEBUG_NOTICE(
			"event '", tinge::fg::make_yellow("CREATE_NOTIFY"),
			"' for '", tinge::fg::make_yellow(fluke::to_hex(win)), "'"
		)


		// Find the currently focused display to launch the new window on.
		FLUKE_DEBUG_NOTICE_SUB("getting focused window and display.")
		const auto [display_x, display_y, display_w, display_h] = fluke::get_hovered_display_rect(conn);


		// Resize window to a percentage of the screen size.
		const auto w = (display_w * fluke::config::NEW_WINDOW_PERCENT) / 100;
		const auto h = (display_h * fluke::config::NEW_WINDOW_PERCENT) / 100;

		// Center the window on the screen.
		const auto x = (display_x + display_w / 2) - w / 2;
		const auto y = (display_y + display_h / 2) - h / 2;

		// Register to receive events from the window and resize/move the window.
		FLUKE_DEBUG_NOTICE_SUB("resising and moving window.")
		fluke::change_window_attributes(conn, win, XCB_CW_EVENT_MASK, fluke::XCB_WINDOW_EVENTS);
		fluke::configure_window(
			conn, win,
			fluke::XCB_MOVE_RESIZE | XCB_CONFIG_WINDOW_BORDER_WIDTH,
			x, y, w, h, config::BORDER_SIZE
		);
	}



	/*
		This event is triggered when a window is destroyed.

		We focus the previous window in the stack ands set its stacking order.
	*/
	inline void event_destroy_notify(fluke::Connection& conn, const fluke::DestroyNotifyEvent& e) {
		const xcb_window_t win = e->window;

		fluke::on_destroy(conn, e);
		FLUKE_DEBUG_NOTICE(
			"event '", tinge::fg::make_yellow("DESTROY_NOTIFY"),
			"' for '", tinge::fg::make_yellow(fluke::to_hex(win)), "'"
		)

		// Focus next alive window
		const auto windows = fluke::get_mapped_windows(conn);

		if (windows.size() == 0)
			return;

		const xcb_window_t new_win = windows.back();

		fluke::configure_window(conn, win, XCB_CONFIG_WINDOW_STACK_MODE, XCB_STACK_MODE_BELOW);
		fluke::configure_window(conn, new_win, XCB_CONFIG_WINDOW_STACK_MODE, XCB_STACK_MODE_ABOVE);

		fluke::set_input_focus(conn, XCB_INPUT_FOCUS_PARENT, new_win);
	}



	/*
		This event is triggered when a window requests to be mapped(made visible).

		We will also set the stacking order and input focus.
	*/
	inline void event_map_request(fluke::Connection& conn, const fluke::MapRequestEvent& e) {
		const xcb_window_t win = e->window;

		fluke::on_map(conn, e);
		FLUKE_DEBUG_NOTICE(
			"event '", tinge::fg::make_yellow("MAP_REQUEST"),
			"' for '", tinge::fg::make_yellow(fluke::to_hex(win)), "'"
		)

		fluke::map_window(conn, win);
		fluke::configure_window(conn, win, XCB_CONFIG_WINDOW_STACK_MODE, XCB_STACK_MODE_ABOVE);
		fluke::set_input_focus(conn, XCB_INPUT_FOCUS_PARENT, win);
	}



	/*
		This event is triggered when a window is unmapped, usually upon its destruction.
	*/
	inline void event_unmap_notify(fluke::Connection& conn, const fluke::UnmapNotifyEvent& e) {
		const xcb_window_t win = e->window;

		fluke::on_unmap(conn, e);
		FLUKE_DEBUG_NOTICE(
			"event '", tinge::fg::make_yellow("UNMAP_NOTIFY"),
			"' for '", tinge::fg::make_yellow(fluke::to_hex(win)), "'"
		)
	}



	/*
		This event is triggered when a window wants to be moved or resized.

		Note that this function can be very hot if used with
		a program like `xmmv` from wmutils.
	*/
	inline void event_configure_request(fluke::Connection& conn, const fluke::ConfigureRequestEvent& e) {
		const xcb_window_t win = e->window;
		const uint16_t mask = e->value_mask;

		fluke::on_configure(conn, e);
		FLUKE_DEBUG_NOTICE(
			"event '", tinge::fg::make_yellow("CONFIGURE_REQUEST"),
			"' for '", tinge::fg::make_yellow(fluke::to_hex(win)), "'"
		)

		// `values` here functions as a stack.
		// We push values onto it depending on if a bitmask is satisfied
		// for each possible option.
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



	/*
		This event is triggered every time the pointer is moved.
		Note that this callback can be very hot.
	*/
	inline void event_motion_notify(fluke::Connection& conn, const fluke::MotionNotifyEvent& e) {
		fluke::on_motion(conn, e);
		FLUKE_DEBUG_NOTICE( "event '", tinge::fg::make_yellow("MOTION_NOTIFY"), "'" )
	}



	/*
		This event is triggered when a property is changed, usually related to ICCCM or EWMH.
	*/
	inline void event_property_notify(fluke::Connection& conn, const fluke::PropertyNotifyEvent& e) {
		fluke::on_property(conn, e);
		FLUKE_DEBUG_NOTICE( "event '", tinge::fg::make_yellow("PROPERTY_NOTIFY"), "'" )
	}



	/*
		This event is triggered when a window sends us a custom message, usally ICCCM or EWMH.
	*/
	inline void event_client_message(fluke::Connection& conn, const fluke::ClientMessageEvent& e) {
		fluke::on_client_message(conn, e);
		FLUKE_DEBUG_NOTICE( "event '", tinge::fg::make_yellow("CLIENT_MESSAGE"), "'" )
	}



	/*
		This event is triggered when a screen(s) is modified. for example, a monitor
		is unplugged or the resolution altered.

		We also move any windows that may be off-screen back into view.
	*/
	inline void event_randr_screen_change_notify(fluke::Connection& conn, const fluke::RandrScreenChangeNotifyEvent& e) {
		fluke::on_randr_screen_change(conn, e);
		FLUKE_DEBUG_NOTICE( "event '", tinge::fg::make_yellow("RANDR_SCREEN_CHANGE_NOTIFY"), "'" )

		// Move windows that are off screen back into view.
	}



	/*
		I'm not actually sure when this gets triggered or why.
	*/
	inline void event_randr_notify(fluke::Connection& conn, const fluke::RandrNotifyEvent& e) {
		fluke::on_randr_notify(conn, e);
		FLUKE_DEBUG_NOTICE( "event '", tinge::fg::make_yellow("RANDR_NOTIFY"), "'" )
	}



	/*
		This event is triggered when a grabbed key is pressed.

		We will call the callback function associated with a keypress we are monitoring.
	*/
	inline void event_keypress(fluke::Connection& conn, const fluke::KeyPressEvent& e) {
		const xcb_keysym_t keysym = fluke::get_keysym(conn, e->detail);

		fluke::on_keypress(conn, e);
		FLUKE_DEBUG_NOTICE( "event '", tinge::fg::make_yellow("KEYPRESS"), "'" )

		// Remove any modifiers from a mask.
		constexpr auto clean = [] (unsigned mask) {
			return mask & ~(fluke::keys::caps_lock | fluke::keys::num_lock | fluke::keys::scroll_lock);
		};

		// Find `fluke::Key` structure which has a matching keysym and modifier.
		for (const auto& [mod, sym, func]: fluke::config::keybindings) {
			if (keysym == sym and clean(mod) == clean(e->state)) {
				func(conn);
				return;
			}
		}
	}



	/*
		This event is triggered when an error occurs, usually when
		another request could not be fulfilled.

		BadWindow errors are expected to happen and as such, are ignored explicitly.
	*/
	inline void event_error(fluke::Connection& conn, const fluke::Error& e) {
		const int major_code = e->major_code;
		const int minor_code = e->minor_code;
		const int error_code = e->error_code;

		// Error code `3` is a bad window error, these errors are pretty much
		// guaranteed to happen and are normal so we wont print them.
		if (error_code == 3)
			return;

		fluke::on_error(conn, e);
		FLUKE_DEBUG_NOTICE( "event '", tinge::fg::make_yellow("ERROR"), "'" )

		// Make error names bright blue.
		const auto major = tinge::fg::bright::make_blue(fluke::request_str[major_code]);
		const auto error = tinge::fg::bright::make_blue(fluke::error_str[error_code]);
		const auto help  = fluke::help_str[error_code];

		// Make error codes dim blue.
		const auto major_code_str = tinge::fg::dim::make_blue(major_code);
		const auto minor_code_str = tinge::fg::dim::make_blue(minor_code);
		const auto error_code_str = tinge::fg::dim::make_blue(error_code);

		// Pretty string with all error codes concatenated.
		const auto all_codes_str = tinge::strcat(
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

