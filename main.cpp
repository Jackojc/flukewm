#include <iostream>
#include <array>
#include <fluke.hpp>


int main() {
	fluke::Connection conn;

	// register to receive window manager events. only one window manager can be active at one time.
	fluke::change_window_attributes(conn, conn.root(), XCB_CW_EVENT_MASK, fluke::XCB_WINDOWMANAGER_EVENTS);

	// adopt any windows which were open at the time of fluke's launch.
	fluke::adopt_orphans(conn);


	// define event handlers, (event_id, handler_name)
	#define EVENT_HANDLERS \
		NEW_HANDLER( 0,                     error             ) \
		NEW_HANDLER( XCB_ENTER_NOTIFY,      enter_notify      ) \
		NEW_HANDLER( XCB_LEAVE_NOTIFY,      leave_notify      ) \
		NEW_HANDLER( XCB_FOCUS_IN,          focus_in          ) \
		NEW_HANDLER( XCB_FOCUS_OUT,         focus_out         ) \
		NEW_HANDLER( XCB_CREATE_NOTIFY,     create_notify     ) \
		NEW_HANDLER( XCB_MAP_REQUEST,       map_request       ) \
		NEW_HANDLER( XCB_CONFIGURE_REQUEST, configure_request )



	// generates a compile time lookup table of labels
	constexpr auto generate_table = [] (const auto& init, const auto& args) {
		using arr_t = std::array<void*, XCB_NO_OPERATION>;
		arr_t labels{};

		for (auto& x: labels)
			x = init;

		for (auto [id, label]: args)
			labels[static_cast<arr_t::size_type>(id)] = label;

		return labels;
	};



	// gets the next event in a blocking fashion
	auto next_event = [&conn] () {
		conn.flush(); // flush pending requests
		auto event = fluke::Event{xcb_wait_for_event(conn)};

		if (xcb_connection_has_error(conn) != 0)
			tinge::errorln("the connection has encountered an error!");

		if (not event)
			tinge::errorln("no event!");

		return event;
	};



	// generate lookup table for labels
	#define NEW_HANDLER(event_id, name) std::pair<decltype(XCB_ENTER_NOTIFY), void*>{ event_id, &&name##_label },
		constexpr static auto labels = generate_table(&&unhandled_label, std::array{ EVENT_HANDLERS });
	#undef NEW_HANDLER



	// return address of label corresponding to event type
	constexpr auto next = [&] (const auto& ev) {
		return labels[XCB_EVENT_RESPONSE_TYPE(ev.get())];
	};



	// jump to initial event handler
	auto old_event = next_event();  // keep old event so it can be passed to handler
	goto* labels[XCB_EVENT_RESPONSE_TYPE(old_event.get())];



	// default case for unhandled events
	unhandled_label:
		FLUKE_DEBUG( tinge::warnln("unhandled event '", fluke::event_str[XCB_EVENT_RESPONSE_TYPE(old_event.get())], "'!") )
		old_event = next_event();
		goto* next(old_event);



	// create labels for all event handlers
	#define NEW_HANDLER(event_id, name) \
		name##_label: \
			fluke::event_handlers::event_##name(conn, std::move(old_event)); \
			old_event = next_event(); \
			goto* next(old_event);

	EVENT_HANDLERS

	#undef NEW_HANDLER


	return 0;
}
