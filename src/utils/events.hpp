#pragma once

#include <utility>
#include <memory>
#include <array>
#include <string_view>
#include <map>

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

	using Error = std::unique_ptr<xcb_generic_error_t>;


	// just a scratch area to store state
	// enum {
	// 	EVENT_COUNT,
	// 	BLACKBOARD_KEYS_TOTAL,
	// };

	// using Blackboard = std::array<int, BLACKBOARD_KEYS_TOTAL>;



	using EventCallback = void(*)(fluke::Connection&, fluke::Event&&);
	using EventEntry = std::pair<int, EventCallback>;
	using EventArray = std::array<EventCallback, XCB_GE_GENERIC>;


}




namespace fluke {

	template <typename... Ts>
	constexpr auto make_event_names(Ts&&... args) {
		using ArrT = std::array<std::string_view, XCB_GE_GENERIC>;

		ArrT names{};
		std::array entries{ std::forward<Ts>(args)... };

		for (auto& x: names)
			x = "UNKNOWN_EVENT";

		for (auto&& [index, value]: entries)
			names[static_cast<ArrT::size_type>(index)] = value;

		return names;
	}


	namespace detail {
		void default_callback(fluke::Connection&, fluke::Event&& e) {
			constexpr static auto event_names = make_event_names(
				std::pair{ 0,                     "EVENT_ERROR"       },
				std::pair{ XCB_CREATE_NOTIFY,     "CREATE_NOTIFY"     },
				std::pair{ XCB_DESTROY_NOTIFY,    "DESTROY_NOTIFY"    },
				std::pair{ XCB_BUTTON_PRESS,      "BUTTON_PRESS"      },
				std::pair{ XCB_BUTTON_RELEASE,    "BUTTON_RELEASE"    },
				std::pair{ XCB_MOTION_NOTIFY,     "MOTION_NOTIFY"     },
				std::pair{ XCB_ENTER_NOTIFY,      "ENTER_NOTIFY"      },
				std::pair{ XCB_CONFIGURE_NOTIFY,  "CONFIGURE_NOTIFY"  },
				std::pair{ XCB_KEY_PRESS,         "KEY_PRESS"         },
				std::pair{ XCB_FOCUS_IN,          "FOCUS_IN"          },
				std::pair{ XCB_FOCUS_OUT,         "FOCUS_OUT"         },
				std::pair{ XCB_KEYMAP_NOTIFY,     "KEYMAP_NOTIFY"     },
				std::pair{ XCB_EXPOSE,            "EXPOSE"            },
				std::pair{ XCB_GRAPHICS_EXPOSURE, "GRAPHICS_EXPOSURE" },
				std::pair{ XCB_NO_EXPOSURE,       "NO_EXPOSURE"       },
				std::pair{ XCB_VISIBILITY_NOTIFY, "VISIBILITY_NOTIFY" },
				std::pair{ XCB_UNMAP_NOTIFY,      "UNMAP_NOTIFY"      },
				std::pair{ XCB_MAP_NOTIFY,        "MAP_NOTIFY"        },
				std::pair{ XCB_MAP_REQUEST,       "MAP_REQUEST"       },
				std::pair{ XCB_REPARENT_NOTIFY,   "REPARENT_NOTIFY"   },
				std::pair{ XCB_CONFIGURE_REQUEST, "CONFIGURE_REQUEST" },
				std::pair{ XCB_GRAVITY_NOTIFY,    "GRAVITY_NOTIFY"    },
				std::pair{ XCB_RESIZE_REQUEST,    "RESIZE_REQUEST"    },
				std::pair{ XCB_CIRCULATE_NOTIFY,  "CIRCULATE_NOTIFY"  },
				std::pair{ XCB_PROPERTY_NOTIFY,   "PROPERTY_NOTIFY"   },
				std::pair{ XCB_SELECTION_CLEAR,   "SELECTION_CLEAR"   },
				std::pair{ XCB_SELECTION_REQUEST, "SELECTION_REQUEST" },
				std::pair{ XCB_SELECTION_NOTIFY,  "SELECTION_NOTIFY"  },
				std::pair{ XCB_COLORMAP_NOTIFY,   "COLORMAP_NOTIFY"   },
				std::pair{ XCB_CLIENT_MESSAGE,    "CLIENT_MESSAGE"    },
				std::pair{ XCB_MAPPING_NOTIFY,    "MAPPING_NOTIFY"    }
			);

			tinge::warnln("unhandled event '", event_names[XCB_EVENT_RESPONSE_TYPE(e.get())], "'!");
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

		if (xcb_connection_has_error(conn) != 0) {
			tinge::errorln("the connection has encountered an error!");
			return false;
		}

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

