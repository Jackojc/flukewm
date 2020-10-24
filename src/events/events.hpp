#ifndef FLUKE_EVENTS_HPP
#define FLUKE_EVENTS_HPP

#pragma once

#include <utility>
#include <memory>
#include <array>
#include <string_view>
#include <map>

#include <fluke.hpp>


namespace fluke {
	#define NEW_EVENT_TYPE(name, type) \
		using name = std::unique_ptr<type, decltype(&std::free)>;

	// event types.
	NEW_EVENT_TYPE(KeyPressEvent, xcb_key_press_event_t)
	NEW_EVENT_TYPE(KeyReleaseEvent, xcb_key_release_event_t)
	NEW_EVENT_TYPE(KeymapNotifyEvent, xcb_keymap_notify_event_t)

	NEW_EVENT_TYPE(ButtonPressEvent, xcb_button_press_event_t)
	NEW_EVENT_TYPE(ButtonReleaseEvent, xcb_button_release_event_t)

	NEW_EVENT_TYPE(MotionNotifyEvent, xcb_motion_notify_event_t)
	NEW_EVENT_TYPE(EnterNotifyEvent, xcb_enter_notify_event_t)
	NEW_EVENT_TYPE(LeaveNotifyEvent, xcb_leave_notify_event_t)

	NEW_EVENT_TYPE(FocusInEvent, xcb_focus_in_event_t)
	NEW_EVENT_TYPE(FocusOutEvent, xcb_focus_out_event_t)

	NEW_EVENT_TYPE(ExposeEvent, xcb_expose_event_t)
	NEW_EVENT_TYPE(GraphicsExposureEvent, xcb_graphics_exposure_event_t)
	NEW_EVENT_TYPE(NoExposureEvent, xcb_no_exposure_event_t)

	NEW_EVENT_TYPE(VisibilityNotifyEvent, xcb_visibility_notify_event_t)
	NEW_EVENT_TYPE(ReparentNotifyEvent, xcb_reparent_notify_event_t)
	NEW_EVENT_TYPE(PropertyNotifyEvent, xcb_property_notify_event_t)
	NEW_EVENT_TYPE(ResizeRequestEvent, xcb_resize_request_event_t)
	NEW_EVENT_TYPE(GravityNotifyEvent, xcb_gravity_notify_event_t)
	NEW_EVENT_TYPE(ColormapNotifyEvent, xcb_colormap_notify_event_t)
	NEW_EVENT_TYPE(ClientMessageEvent, xcb_client_message_event_t)

	NEW_EVENT_TYPE(CreateNotifyEvent, xcb_create_notify_event_t)
	NEW_EVENT_TYPE(DestroyNotifyEvent, xcb_destroy_notify_event_t)

	NEW_EVENT_TYPE(UnmapNotifyEvent, xcb_unmap_notify_event_t)
	NEW_EVENT_TYPE(MapNotifyEvent, xcb_map_notify_event_t)
	NEW_EVENT_TYPE(MapRequestEvent, xcb_map_request_event_t)
	NEW_EVENT_TYPE(MappingNotifyEvent, xcb_mapping_notify_event_t)

	NEW_EVENT_TYPE(ConfigureNotifyEvent, xcb_configure_notify_event_t)
	NEW_EVENT_TYPE(ConfigureRequestEvent, xcb_configure_request_event_t)

	NEW_EVENT_TYPE(CirculateRequestEvent, xcb_circulate_request_event_t)
	NEW_EVENT_TYPE(CirculateNotifyEvent, xcb_circulate_notify_event_t)

	NEW_EVENT_TYPE(SelectionClearEvent, xcb_selection_clear_event_t)
	NEW_EVENT_TYPE(SelectionRequestEvent, xcb_selection_request_event_t)
	NEW_EVENT_TYPE(SelectionNotifyEvent, xcb_selection_notify_event_t)

	NEW_EVENT_TYPE(RandrScreenChangeNotifyEvent, xcb_randr_screen_change_notify_event_t)
	NEW_EVENT_TYPE(RandrNotifyEvent, xcb_randr_notify_event_t)

	NEW_EVENT_TYPE(GeEvent, xcb_ge_generic_event_t)
	NEW_EVENT_TYPE(Event, xcb_generic_event_t)

	NEW_EVENT_TYPE(Error, xcb_generic_error_t)

	#undef NEW_EVENT_TYPE
}





// functions to cast between event types
namespace fluke {
	// Use custom deleter
	template <typename To, typename From, typename Del>
	inline decltype(auto) event_cast(std::unique_ptr<From, Del>&& ptr) {
		return std::unique_ptr<typename To::element_type, Del>{
			reinterpret_cast<typename To::element_type*>(ptr.release()), std::move(ptr.get_deleter())
		};
	}


	// Use default deleter
	template <typename To, typename From>
	inline decltype(auto) event_cast(std::unique_ptr<From>&& ptr) {
		return std::unique_ptr<typename To::element_type>{
			reinterpret_cast<typename To::element_type*>(ptr.release())
		};
	}
}

#endif
