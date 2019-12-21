#pragma once

#include <utility>
#include <memory>
#include <array>

#include <fluke.hpp>

namespace fluke {
	// event types.
	using KeyPressEvent = std::unique_ptr<xcb_key_press_event_t>;
	using KeyReleaseEvent = std::unique_ptr<xcb_key_release_event_t>;
	using KeymapNotifyEvent = std::unique_ptr<xcb_keymap_notify_event_t>;

	using ButtonPressEvent = std::unique_ptr<xcb_button_press_event_t>;
	using ButtonReleaseEvent = std::unique_ptr<xcb_button_release_event_t>;

	using MotionNotifyEvent = std::unique_ptr<xcb_motion_notify_event_t>;
	using EnterNotifyEvent = std::unique_ptr<xcb_enter_notify_event_t>;
	using LeaveNotifyEvent = std::unique_ptr<xcb_leave_notify_event_t>;

	using FocusInEvent = std::unique_ptr<xcb_focus_in_event_t>;
	using FocusOutEvent = std::unique_ptr<xcb_focus_out_event_t>;

	using ExposeEvent = std::unique_ptr<xcb_expose_event_t>;
	using GraphicsExposureEvent = std::unique_ptr<xcb_graphics_exposure_event_t>;
	using NoExposureEvent = std::unique_ptr<xcb_no_exposure_event_t>;

	using VisibilityNotifyEvent = std::unique_ptr<xcb_visibility_notify_event_t>;
	using ReparentNotifyEvent = std::unique_ptr<xcb_reparent_notify_event_t>;
	using PropertyNotifyEvent = std::unique_ptr<xcb_property_notify_event_t>;
	using ResizeRequestEvent = std::unique_ptr<xcb_resize_request_event_t>;
	using GravityNotifyEvent = std::unique_ptr<xcb_gravity_notify_event_t>;
	using ColormapNotifyEvent = std::unique_ptr<xcb_colormap_notify_event_t>;
	using ClientMessageEvent = std::unique_ptr<xcb_client_message_event_t>;

	using CreateNotifyEvent = std::unique_ptr<xcb_create_notify_event_t>;
	using DestroyNotifyEvent = std::unique_ptr<xcb_destroy_notify_event_t>;

	using UnmapNotifyEvent = std::unique_ptr<xcb_unmap_notify_event_t>;
	using MapNotifyEvent = std::unique_ptr<xcb_map_notify_event_t>;
	using MapRequestEvent = std::unique_ptr<xcb_map_request_event_t>;
	using MappingNotifyEvent = std::unique_ptr<xcb_mapping_notify_event_t>;

	using ConfigureNotifyEvent = std::unique_ptr<xcb_configure_notify_event_t>;
	using ConfigureRequestEvent = std::unique_ptr<xcb_configure_request_event_t>;

	using CirculateRequestEvent = std::unique_ptr<xcb_circulate_request_event_t>;
	using CirculateNotifyEvent = std::unique_ptr<xcb_circulate_notify_event_t>;

	using SelectionClearEvent = std::unique_ptr<xcb_selection_clear_event_t>;
	using SelectionRequestEvent = std::unique_ptr<xcb_selection_request_event_t>;
	using SelectionNotifyEvent = std::unique_ptr<xcb_selection_notify_event_t>;

	using GeEvent = std::unique_ptr<xcb_ge_generic_event_t>;
	using Event = std::unique_ptr<xcb_generic_event_t>;

	using EventCallback = void(*)(fluke::Connection&, fluke::Event&&);
	using EventEntry = std::pair<int, EventCallback>;
	using EventArray = std::array<EventCallback, XCB_GE_GENERIC>;
}




namespace fluke {

	namespace detail {
		void default_callback(fluke::Connection&, fluke::Event&&) {
			tinge::warnln("unhandled event!");
		};
	}


	template <typename... Ts>
	constexpr auto make_events(Ts&&... args) {
		using ArrT = std::array<fluke::EventCallback, XCB_GE_GENERIC>;

		ArrT events{};
		std::array<fluke::EventEntry, sizeof...(Ts)> entries{ std::forward<Ts>(args)... };

		for (auto& x: events)
			x = &detail::default_callback;

		for (auto&& [index, value]: entries)
			events[static_cast<ArrT::size_type>(index)] = value;

		return events;
	}


	bool handle_events(fluke::Connection& conn, const EventArray& events) {
		auto event = fluke::Event{xcb_wait_for_event(conn)};

		// if (xcb_connection_has_error(conn) != 0) {
		// 	tinge::errorln("the connection has encountered an error!");
		// 	return false;
		// }

		if (event)
			events.at(XCB_EVENT_RESPONSE_TYPE(event.get()))(conn, std::move(event));

		return true;
	}
}





namespace fluke {
	// Use custom deleter
	template <typename To, typename From, typename Del>
	decltype(auto) event_cast(std::unique_ptr<From, Del>&& ptr) {
		return std::unique_ptr<typename To::element_type, Del>{
			reinterpret_cast<typename To::element_type*>(ptr.release()), std::move(ptr.get_deleter())
		};
	}


	// Use default deleter
	template <typename To, typename From>
	decltype(auto) event_cast(std::unique_ptr<From>&& ptr) {
		return std::unique_ptr<typename To::element_type>{
			reinterpret_cast<typename To::element_type*>(ptr.release())
		};
	}
}

