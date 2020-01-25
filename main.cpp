#include <iostream>
#include <array>
#include <cstdlib>
#include <csignal>
#include <csetjmp>
#include <fluke.hpp>


std::jmp_buf exit_jump;     // This is used to jump back to main after a signal is handled.
int status = EXIT_SUCCESS;  // Exit status for signal handler.


// Keyboard interrupt.
void sigint(int) {
	FLUKE_DEBUG_WARN("SIGINT")
	std::longjmp(exit_jump, EXIT_FAILURE);
}


// Terminate.
void sigterm(int) {
	FLUKE_DEBUG_WARN("SIGTERM")
	std::longjmp(exit_jump, EXIT_FAILURE);
}


// Kill.
[[noreturn]] void sigkill(int) {
	FLUKE_DEBUG_ERROR("SIGKILL")
	std::exit(EXIT_FAILURE);
}



int main() {
	// Setup signals handlers.
	FLUKE_DEBUG_SUCCESS("setting up signal handlers.")
	std::signal(SIGINT, sigint);
	std::signal(SIGTERM, sigterm);
	std::signal(SIGKILL, sigkill);


	// Connect to X.
	FLUKE_DEBUG_SUCCESS("connecting to X server.")
	fluke::Connection conn;


	// Setup the randr extension to allow us to recieve display change events.
	FLUKE_DEBUG_SUCCESS("setting up randr extension.")

	const auto randr_ext = xcb_get_extension_data(conn, &xcb_randr_id);
	const auto randr_base = randr_ext->first_event;

	fluke::randr_select_input(conn, conn.root(),
		XCB_RANDR_NOTIFY_MASK_SCREEN_CHANGE   |
		XCB_RANDR_NOTIFY_MASK_OUTPUT_CHANGE   |
		XCB_RANDR_NOTIFY_MASK_CRTC_CHANGE     |
		XCB_RANDR_NOTIFY_MASK_OUTPUT_PROPERTY
	);


	// Register to receive window manager events. Only one window manager can be active at one time.
	FLUKE_DEBUG_SUCCESS("registering as a window manager.")
	fluke::change_window_attributes(conn, conn.root(), XCB_CW_EVENT_MASK, fluke::XCB_WINDOWMANAGER_EVENTS);


	// Gain control of windows which were already open before the window manager was started
	// so that we can receive events for them.
	FLUKE_DEBUG_SUCCESS("adopting orphaned windows.")

	// For every mapped window, tell it what events we wish to receive from it
	// and also set the border colour and width of the window.
	for (xcb_window_t win: fluke::get_mapped_windows(conn)) {
		fluke::change_window_attributes(conn, win, XCB_CW_EVENT_MASK, fluke::XCB_WINDOW_EVENTS);
		fluke::configure_window(conn, win, XCB_CONFIG_WINDOW_BORDER_WIDTH, fluke::config::BORDER_SIZE);
		fluke::change_window_attributes(conn, win, XCB_CW_BORDER_PIXEL, fluke::config::BORDER_COLOUR_INACTIVE);
	}


	// Get the window which currently has keyboard focus
	// (if no window is focused an error will be generated but we just ignore it)
	xcb_window_t focused = fluke::get_focused_window(conn);

	// Set the stacking mode, border width and border colour for the focused window.
	fluke::configure_window(conn, focused, XCB_CONFIG_WINDOW_BORDER_WIDTH | XCB_CONFIG_WINDOW_STACK_MODE, fluke::config::BORDER_SIZE, XCB_STACK_MODE_ABOVE);
	fluke::change_window_attributes(conn, focused, XCB_CW_BORDER_PIXEL, fluke::config::BORDER_COLOUR_ACTIVE);


	// Register keybindings defined in the `keys` structure of the config.
	if constexpr(fluke::config::keybindings.size() > 0) {
		FLUKE_DEBUG_SUCCESS("registering keybindings.")
		fluke::register_keybindings(conn, fluke::config::keybindings);
	}


	// Set jump point, when a signal handler gets activated, it will jump here.
	if (status = setjmp(exit_jump); status) {
		FLUKE_DEBUG_SUCCESS("signal caught.")
		goto exit;
	}


	FLUKE_DEBUG_SUCCESS("starting main event loop.")
	fluke::on_launch(conn);

	while (true) {
		conn.flush();

		// Get the next event and its type.
		auto event = fluke::get_next_event(conn);
		auto ev_type = fluke::get_event_type(event);
		auto randr_ev_type = randr_base - ev_type;

		// Check for errors.
		if (xcb_connection_has_error(conn) != 0) {
			tinge::errorln("cannot connect to the X server!");
			break;
		}

		if (not event) {
			tinge::errorln("event returned nullptr, this shouldnt happen!");
			continue;
		}


		// Handle all events.
		switch (ev_type) {
			case 0:
				fluke::event_error(conn,
					fluke::event_cast<fluke::Error>(std::move(event))
				);
				continue;

			case XCB_ENTER_NOTIFY:
				fluke::event_enter_notify(conn,
					fluke::event_cast<fluke::EnterNotifyEvent>(std::move(event))
				);
				continue;

			case XCB_LEAVE_NOTIFY:
				fluke::event_leave_notify(conn,
					fluke::event_cast<fluke::LeaveNotifyEvent>(std::move(event))
				);
				continue;

			case XCB_FOCUS_IN:
				fluke::event_focus_in(conn,
					fluke::event_cast<fluke::FocusInEvent>(std::move(event))
				);
				continue;

			case XCB_FOCUS_OUT:
				fluke::event_focus_out(conn,
					fluke::event_cast<fluke::FocusOutEvent>(std::move(event))
				);
				continue;

			case XCB_CREATE_NOTIFY:
				fluke::event_create_notify(conn,
					fluke::event_cast<fluke::CreateNotifyEvent>(std::move(event))
				);
				continue;

			case XCB_DESTROY_NOTIFY:
				fluke::event_destroy_notify(conn,
					fluke::event_cast<fluke::DestroyNotifyEvent>(std::move(event))
				);
				continue;

			case XCB_MAP_REQUEST:
				fluke::event_map_request(conn,
					fluke::event_cast<fluke::MapRequestEvent>(std::move(event))
				);
				continue;

			case XCB_UNMAP_NOTIFY:
				fluke::event_unmap_notify(conn,
					fluke::event_cast<fluke::UnmapNotifyEvent>(std::move(event))
				);
				continue;

			case XCB_CONFIGURE_REQUEST:
				fluke::event_configure_request(conn,
					fluke::event_cast<fluke::ConfigureRequestEvent>(std::move(event))
				);
				continue;

			case XCB_KEY_PRESS:
				fluke::event_keypress(conn,
					fluke::event_cast<fluke::KeyPressEvent>(std::move(event))
				);
				continue;

			case XCB_PROPERTY_NOTIFY:
				fluke::event_property_notify(conn,
					fluke::event_cast<fluke::PropertyNotifyEvent>(std::move(event))
				);
				continue;

			case XCB_CLIENT_MESSAGE:
				fluke::event_client_message(conn,
					fluke::event_cast<fluke::ClientMessageEvent>(std::move(event))
				);
				continue;
		}


		// Handle randr events, these checks are exhaustive so we
		// do not need to check for unhandled randr events.
		switch (randr_ev_type) {
			case XCB_RANDR_SCREEN_CHANGE_NOTIFY:
				fluke::event_randr_screen_change_notify(conn,
					fluke::event_cast<fluke::RandrScreenChangeNotifyEvent>(std::move(event))
				);
				continue;

			case XCB_RANDR_NOTIFY:
				fluke::event_randr_notify(conn,
					fluke::event_cast<fluke::RandrNotifyEvent>(std::move(event))
				);
				continue;
		}


		// Warn about unhandled events.
		FLUKE_DEBUG_WARN("unhandled event '", fluke::event_str[ev_type], "'!")
	}

	exit:

	FLUKE_DEBUG_SUCCESS("exiting.")
	fluke::on_exit(conn);

	return status;
}
