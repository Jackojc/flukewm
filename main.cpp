#include <iostream>
#include <optional>
#include <functional>
#include <chrono>
#include <thread>
#include <vector>
#include <array>
#include <variant>
#include <tuple>
#include <utility>
#include <algorithm>

#include <tinge.hpp>
#include <fluke.hpp>



namespace fluke::constants {
	// argb format
	constexpr uint32_t BORDER_COLOUR_INACTIVE = 0xff000000;
	constexpr uint32_t BORDER_COLOUR_ACTIVE   = 0xffffffff;

	constexpr uint32_t BORDER_SIZE = 1;
}


namespace fluke {
	using HookCallback = void(*)(fluke::Connection&);
	using HookEntry = std::pair<int, HookCallback>;
	template <size_t N> using HookArray = std::array<HookEntry, N>;



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



namespace fluke {
	bool handle_events(fluke::Connection& conn, const EventArray& events) {
		auto event = fluke::Event{xcb_poll_for_event(conn)};

		if (xcb_connection_has_error(conn) != 0) {
			tinge::errorln("the connection has encountered an error!");
			return false;
		}

		if (event)
			events.at(XCB_EVENT_RESPONSE_TYPE(event.get()))(conn, std::move(event));

			// callback(std::move(event));

		return true;
	}


	template <size_t N>
	void handle_hooks(fluke::Connection& conn, const HookArray<N>& hooks, int current_ms) {
		if constexpr(hooks.size() > 0) {
			for (auto&& [ms, func]: hooks) {
				if (current_ms % ms == 0) {
					func(conn);
				}
			}
		}
	}
}



namespace fluke {
	auto get_all_windows(fluke::Connection& conn) {
		// Get all windows.
		auto tree = fluke::GetTree{conn, conn.root()}.get();
		std::vector<xcb_window_t> windows{
			xcb_query_tree_children(tree.get()),  // pointer to array of windows.
			xcb_query_tree_children(tree.get()) + xcb_query_tree_children_length(tree.get())
		};

		// Get window attributes.
		auto attrs = fluke::RequestContainer<fluke::GetWindowAttributes>{conn, windows}.get();

		// Ignore windows which have override_redirect set.
		decltype(attrs)::size_type i = 0;
		windows.erase(std::remove_if(windows.begin(), windows.end(), [&] (xcb_window_t) {
			return attrs[i++]->override_redirect;
		}));

		return windows;
	}


	auto get_mapped_windows(fluke::Connection& conn) {
		// Get all windows.
		auto tree = fluke::GetTree{conn, conn.root()}.get();
		std::vector<xcb_window_t> windows{
			xcb_query_tree_children(tree.get()),  // pointer to array of windows.
			xcb_query_tree_children(tree.get()) + xcb_query_tree_children_length(tree.get())
		};

		// Get window attributes.
		auto attrs = fluke::RequestContainer<fluke::GetWindowAttributes>{conn, windows}.get();

		// Ignore windows which have override_redirect set.
		decltype(attrs)::size_type i = 0;
		windows.erase(std::remove_if(windows.begin(), windows.end(), [&] (xcb_window_t) {
			bool flag = not attrs[i]->override_redirect and attrs[i]->map_state == XCB_MAP_STATE_VIEWABLE;
			i++;
			return not flag;
		}));

		return windows;
	}
}



namespace fluke {
	template <typename... Ts>
	constexpr auto make_hooks(Ts&&... args) {
		return HookArray<sizeof...(Ts)>{ std::forward<Ts>(args)... };
	}


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
}



namespace fluke {
	void adopt_orphaned_windows(fluke::Connection& conn) {
		auto windows = fluke::get_mapped_windows(conn);

		// register events on windows.
		fluke::RequestContainer<fluke::SetWindowAttributes> request_events{
			conn, windows, XCB_CW_EVENT_MASK, &fluke::XCB_WINDOW_EVENTS
		};

		// Set border width.
		fluke::RequestContainer<fluke::SetWindowConfig> request_borders{
			conn, windows, XCB_CONFIG_WINDOW_BORDER_WIDTH, fluke::data{constants::BORDER_SIZE}
		};

		// Set border colour.
		fluke::RequestContainer<fluke::SetWindowAttributes> request_colours{
			conn, windows, XCB_CW_BORDER_PIXEL, fluke::data{constants::BORDER_COLOUR_INACTIVE}
		};

		// commit changes
		fluke::get(request_events, request_colours, request_borders);


		// highlight currently focused window and set stacking order.
		xcb_window_t focused = fluke::GetInputFocus{conn}.get()->focus; // get currently focused window
		uint32_t values[] = { constants::BORDER_SIZE, XCB_STACK_MODE_ABOVE };

		fluke::RequestBuffer{
			fluke::SetWindowConfig{
				conn, focused, XCB_CONFIG_WINDOW_BORDER_WIDTH | XCB_CONFIG_WINDOW_STACK_MODE, values
			},
			fluke::SetWindowAttributes{conn, focused, XCB_CW_BORDER_PIXEL, fluke::data{constants::BORDER_COLOUR_ACTIVE}}		}.get();
	}
}


namespace fluke::event_handlers {
	// void(*)(fluke::Connection&, fluke::Event&&);

	void event_enter(fluke::Connection& conn, fluke::Event&& e_) {
		auto e = fluke::event_cast<fluke::EnterNotifyEvent>(std::move(e_));
		xcb_window_t win = e->event;

		// respect ignored windows
		if (fluke::GetWindowAttributes{conn, win}.get()->override_redirect)
			return;

		// set input focus to new window, set borders and stacking order.
		fluke::RequestBuffer{
			fluke::SetInputFocus{conn, XCB_INPUT_FOCUS_NONE, win, XCB_CURRENT_TIME},
			fluke::SetWindowConfig{conn, win, XCB_CONFIG_WINDOW_STACK_MODE, fluke::data{XCB_STACK_MODE_ABOVE}}
		}.get();
	}



	void event_leave(fluke::Connection& conn, fluke::Event&& e_) {
		auto e = fluke::event_cast<fluke::LeaveNotifyEvent>(std::move(e_));
		xcb_window_t win = e->event;

		// respect ignored windows
		if (fluke::GetWindowAttributes{conn, win}.get()->override_redirect)
			return;

		// set input focus, set borders
		fluke::SetInputFocus{conn, XCB_INPUT_FOCUS_NONE, XCB_NONE, XCB_CURRENT_TIME}.get();
	}



	void event_focus_in(fluke::Connection& conn, fluke::Event&& e_) {
		auto e = fluke::event_cast<fluke::FocusInEvent>(std::move(e_));
		xcb_window_t win = e->event;

		// respect ignored windows
		if (fluke::GetWindowAttributes{conn, win}.get()->override_redirect)
			return;

		// set borders
		fluke::RequestBuffer{
			fluke::SetWindowConfig{conn, win, XCB_CONFIG_WINDOW_BORDER_WIDTH, fluke::data{constants::BORDER_SIZE}},
			fluke::SetWindowAttributes{conn, win, XCB_CW_BORDER_PIXEL, fluke::data{constants::BORDER_COLOUR_ACTIVE}}
		}.get();
	}



	void event_focus_out(fluke::Connection& conn, fluke::Event&& e_) {
		auto e = fluke::event_cast<fluke::FocusOutEvent>(std::move(e_));
		xcb_window_t win = e->event;

		if (fluke::GetWindowAttributes{conn, win}.get()->override_redirect)
			return;

		// set borders.
		fluke::RequestBuffer{
			fluke::SetWindowConfig{conn, win, XCB_CONFIG_WINDOW_BORDER_WIDTH, fluke::data{constants::BORDER_SIZE}},
			fluke::SetWindowAttributes{conn, win, XCB_CW_BORDER_PIXEL, fluke::data{constants::BORDER_COLOUR_INACTIVE}}
		}.get();
	}
}





int main() {
	try {
		using namespace std::chrono_literals;

		fluke::Connection conn;

		// register to receive window manager events. only one window manager can be active at one time.
		fluke::SetWindowAttributes{conn, conn.root(), XCB_CW_EVENT_MASK, &fluke::XCB_WINDOWMANAGER_EVENTS}.get();

		// adopt any windows which were open at the time of fluke's launch.
		fluke::adopt_orphaned_windows(conn);


		constexpr auto hooks = fluke::make_hooks(
			// fluke::HookEntry{ 1000, &test2 },
		);

		constexpr auto events = fluke::make_events(
			fluke::EventEntry{ XCB_ENTER_NOTIFY, &fluke::event_handlers::event_enter     },
			fluke::EventEntry{ XCB_LEAVE_NOTIFY, &fluke::event_handlers::event_leave     },
			fluke::EventEntry{ XCB_FOCUS_IN,     &fluke::event_handlers::event_focus_in  },
			fluke::EventEntry{ XCB_FOCUS_OUT,    &fluke::event_handlers::event_focus_out }
		);



		bool running = true;

		for (int current_ms = 0; running; current_ms++) {
			// handle events (if there are any)
			running = fluke::handle_events(conn, events);

			// handle user defined hooks.
			fluke::handle_hooks(conn, hooks, current_ms);

			// dont kill the cpu
			std::this_thread::sleep_for(10ms);
		}


	} catch (const fluke::SetWindowAttributesError& e) {
		tinge::errorln("fluke: another window manager is already running!");
		return 1;

	} catch (const fluke::ConnectionError& e) {
		tinge::errorln("fluke: cannot connect to X!");
		return 2;

	} catch (const fluke::ScreenError& e) {
		tinge::errorln("fluke: cannot get screen information!");
		return 2;
	}

	return 1;
}
