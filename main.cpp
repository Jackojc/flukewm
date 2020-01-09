#include <iostream>
#include <array>
#include <cstdlib>
#include <csignal>
#include <csetjmp>
#include <fluke.hpp>


std::jmp_buf exit_jump;  // This is used to jump back to main after a signal is handled.
int status;  // Exit status for signal handler.


// Keyboard interrupt.
void sigint(int) {
	FLUKE_DEBUG( tinge::warnln("SIGINT") )
	std::longjmp(exit_jump, EXIT_FAILURE);
}


// Terminate.
void sigterm(int) {
	FLUKE_DEBUG( tinge::warnln("SIGTERM") )
	std::longjmp(exit_jump, EXIT_FAILURE);
}


// Kill.
void sigkill(int) {
	std::exit(EXIT_FAILURE);
}



int main() {
	// Setup signals handlers.
	std::signal(SIGINT, sigint);
	std::signal(SIGTERM, sigterm);
	std::signal(SIGKILL, sigkill);


	// Connect to X.
	FLUKE_DEBUG( tinge::successln("connect") )
	fluke::Connection conn;


	// Register to receive window manager events. Only one window manager can be active at one time.
	FLUKE_DEBUG( tinge::successln("register as window manager") )
	fluke::change_window_attributes(conn, conn.root(), XCB_CW_EVENT_MASK, fluke::XCB_WINDOWMANAGER_EVENTS);


	// Adopt any windows which were open at the time of fluke's launch.
	FLUKE_DEBUG( tinge::successln("adopt orphans") )
	fluke::adopt_orphans(conn);


	// Register keybindings defined in the `keys` structure of the config.
	fluke::register_keybindings(conn, fluke::config::keys);


	// Define event handlers. NEW_HANDLER(event_id, handler_name)
	#define EVENT_HANDLERS \
		NEW_HANDLER( 0,                     event_error             ) \
		NEW_HANDLER( XCB_ENTER_NOTIFY,      event_enter_notify      ) \
		NEW_HANDLER( XCB_LEAVE_NOTIFY,      event_leave_notify      ) \
		NEW_HANDLER( XCB_FOCUS_IN,          event_focus_in          ) \
		NEW_HANDLER( XCB_FOCUS_OUT,         event_focus_out         ) \
		NEW_HANDLER( XCB_CREATE_NOTIFY,     event_create_notify     ) \
		NEW_HANDLER( XCB_DESTROY_NOTIFY,    event_destroy_notify    ) \
		NEW_HANDLER( XCB_MAP_REQUEST,       event_map_request       ) \
		NEW_HANDLER( XCB_UNMAP_NOTIFY,      event_unmap_notify      ) \
		NEW_HANDLER( XCB_CONFIGURE_REQUEST, event_configure_request ) \
		NEW_HANDLER( XCB_KEY_PRESS,         event_keypress          )


	// Returns a lookup table.
	constexpr auto generate_table = [] (const auto& init, const auto& args) {
		using arr_t = std::array<void*, XCB_NO_OPERATION>;
		arr_t labels{};

		for (auto& x: labels)
			x = init;

		for (auto [id, label]: args)
			labels[static_cast<arr_t::size_type>(id)] = label;

		return labels;
	};


	// Generate a lookup table that maps event types -> labels.
	#define NEW_HANDLER(event_id, name) std::pair<decltype(XCB_ENTER_NOTIFY), void*>{ event_id, &&name##_label },
		constexpr auto labels = generate_table(&&unhandled_label, std::array{ EVENT_HANDLERS });
	#undef NEW_HANDLER


	// Get the next event (blocking).
	auto next_event = [&conn] () {
		conn.flush(); // Flush pending requests.

		auto event = fluke::Event{xcb_wait_for_event(conn)};

		// Check for errors.
		if (xcb_connection_has_error(conn) != 0)
			tinge::errorln("cannot connect to the X server!");

		if (not event)
			tinge::errorln("event returned nullptr, this shouldnt happen!");

		return event;
	};


	// Retrieve first event.
	FLUKE_DEBUG( tinge::successln("starting main event loop") )
	auto old_event = next_event();  // keep old event so it can be passed to handler


	// Set jump point, when a signal handler gets activated, it will jump here.
	if (status = setjmp(exit_jump); status) {
		FLUKE_DEBUG( tinge::warnln("jumping to exit") )
		goto exit;
	}


	// Handle first event.
	goto* labels[XCB_EVENT_RESPONSE_TYPE(old_event.get())];


	// Default handler.
	unhandled_label:
		FLUKE_DEBUG( tinge::warnln("unhandled event '", fluke::event_str[XCB_EVENT_RESPONSE_TYPE(old_event.get())], "'!") )
		old_event = next_event();
		goto* labels[XCB_EVENT_RESPONSE_TYPE(old_event.get())];


	// Create labels and handlers.
	#define NEW_HANDLER(event_id, name) \
		name##_label: \
			fluke::name(conn, std::move(old_event)); \
			old_event = next_event(); \
			goto* labels[XCB_EVENT_RESPONSE_TYPE(old_event.get())];

		EVENT_HANDLERS

	#undef NEW_HANDLER


	// Exit.
	exit:
		FLUKE_DEBUG( tinge::successln("clean exit") )

	return EXIT_SUCCESS;
}
