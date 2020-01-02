#pragma once

#include <vector>
#include <algorithm>
#include <sstream>
#include <string>

#include <fluke.hpp>

namespace fluke {
	#ifdef NDEBUG
		#define FLUKE_DEBUG(x) { }
	#else
		#define FLUKE_DEBUG(x) { x; }
	#endif



	template <typename T>
	inline std::string to_hex(T&& arg) {
		std::stringstream ss;
		ss << "0x" << std::hex << arg;
		return ss.str();
	}



	// Fire off a bunch of requests then get the reply to all of them.
	template <typename T, typename F, typename... Ts>
	inline auto dispatch_consume(fluke::Connection& conn, F func, const std::vector<T>& changing_arg, Ts&&... args) {
		std::vector<decltype(func(changing_arg.at(0), std::forward<Ts>(args)...))> request;
		std::vector<decltype(fluke::get(conn, request.front()))> reply;

		for (auto x: changing_arg)
			request.emplace_back(func(x, std::forward<Ts>(args)...));

		for (auto x: request)
			reply.emplace_back(fluke::get(conn, x));

		return reply;
	}


	inline auto get_all_windows(fluke::Connection& conn) {
		// Get all windows.
		auto tree = fluke::get(conn, fluke::query_tree(conn, conn.root()));

		std::vector<xcb_window_t> windows{
			xcb_query_tree_children(tree.get()),  // pointer to array of windows.
			xcb_query_tree_children(tree.get()) + xcb_query_tree_children_length(tree.get())
		};


		// Get window attributes.
		auto attrs = fluke::dispatch_consume(conn, [] (auto&& win, auto&&... args) {
			return fluke::get_window_attributes( args..., win );
		}, windows, conn);


		// Ignore windows which have override_redirect set.
		decltype(attrs)::size_type i = 0;
		windows.erase(std::remove_if(windows.begin(), windows.end(), [&] (xcb_window_t) {
			return attrs[i++]->override_redirect;
		}), windows.end());

		return windows;
	}


	inline auto get_mapped_windows(fluke::Connection& conn) {
		// Get all windows.
		auto tree = fluke::get(conn, fluke::query_tree(conn, conn.root()));

		std::vector<xcb_window_t> windows{
			xcb_query_tree_children(tree.get()),  // pointer to array of windows.
			xcb_query_tree_children(tree.get()) + xcb_query_tree_children_length(tree.get())
		};


		// Get window attributes.
		auto attrs = fluke::dispatch_consume(conn, [] (auto&& win, auto&&... args) {
			return fluke::get_window_attributes( args..., win );
		}, windows, conn);


		// Ignore windows which have override_redirect set.
		decltype(attrs)::size_type i = 0;
		windows.erase(std::remove_if(windows.begin(), windows.end(), [&] (xcb_window_t) {
			bool flag = not attrs.at(i)->override_redirect and attrs.at(i)->map_state == XCB_MAP_STATE_VIEWABLE;
			i++;
			return not flag;
		}), windows.end());

		return windows;
	}



	inline void adopt_orphans(fluke::Connection& conn) {
		// register events and set border width & colour.
		for (auto&& win: fluke::get_mapped_windows(conn)) {
			fluke::change_window_attributes(conn, win, XCB_CW_EVENT_MASK, fluke::XCB_WINDOW_EVENTS);
			fluke::configure_window(conn, win, XCB_CONFIG_WINDOW_BORDER_WIDTH, config::BORDER_SIZE);
			fluke::change_window_attributes(conn, win, XCB_CW_BORDER_PIXEL, config::BORDER_COLOUR_INACTIVE);
		}

		// get currently focused window
		xcb_window_t focused = fluke::get(conn, fluke::get_input_focus(conn))->focus; // get currently focused window

		if (focused != conn.root()) {
			fluke::configure_window(conn, focused, XCB_CONFIG_WINDOW_BORDER_WIDTH | XCB_CONFIG_WINDOW_STACK_MODE, config::BORDER_SIZE, XCB_STACK_MODE_ABOVE);
			fluke::change_window_attributes(conn, focused, XCB_CW_BORDER_PIXEL, config::BORDER_COLOUR_ACTIVE);
		}
	}

}

