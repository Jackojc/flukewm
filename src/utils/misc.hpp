#pragma once

#include <vector>
#include <algorithm>

#include <fluke.hpp>

namespace fluke {
	#define FLUKE_DEBUG(x) if constexpr(constants::DEBUG) { x; }


	// Fire off a bunch of requests then get the reply to all of them.
	template <typename T, typename F, typename... Ts>
	auto dispatch_consume(F func, const std::vector<T>& changing_arg, Ts&&... args) {
		std::vector<decltype(func(changing_arg.at(0), std::forward<Ts>(args)...))> request;
		std::vector<decltype(request.front().get())> reply;

		for (auto x: changing_arg)
			request.emplace_back(func(x, std::forward<Ts>(args)...));

		for (auto x: request)
			reply.emplace_back(x.get());

		return reply;
	}


	auto get_all_windows(fluke::Connection& conn) {
		FLUKE_DEBUG( tinge::warnln("get_all_windows") )

		// Get all windows.
		auto tree = fluke::QueryTree{conn, conn.root()}.get();

		std::vector<xcb_window_t> windows{
			xcb_query_tree_children(tree.get()),  // pointer to array of windows.
			xcb_query_tree_children(tree.get()) + xcb_query_tree_children_length(tree.get())
		};


		// Get window attributes.
		auto attrs = fluke::dispatch_consume([] (auto&& win, auto&&... args) {
			return fluke::GetWindowAttributes{ args..., win };
		}, windows, conn);


		// Ignore windows which have override_redirect set.
		decltype(attrs)::size_type i = 0;
		windows.erase(std::remove_if(windows.begin(), windows.end(), [&] (xcb_window_t) {
			return attrs[i++]->override_redirect;
		}), windows.end());

		return windows;
	}


	auto get_mapped_windows(fluke::Connection& conn) {
		FLUKE_DEBUG( tinge::warnln("get_mapped_windows") )

		// Get all windows.
		auto tree = fluke::QueryTree{conn, conn.root()}.get();

		std::vector<xcb_window_t> windows{
			xcb_query_tree_children(tree.get()),  // pointer to array of windows.
			xcb_query_tree_children(tree.get()) + xcb_query_tree_children_length(tree.get())
		};


		// Get window attributes.
		auto attrs = fluke::dispatch_consume([] (auto&& win, auto&&... args) {
			return fluke::GetWindowAttributes{ args..., win };
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



	void adopt_orphaned_windows(fluke::Connection& conn) {
		FLUKE_DEBUG( tinge::warnln("adopt_orphaned_windows") )

		// register events and set border width & colour.
		for (auto&& win: fluke::get_mapped_windows(conn)) {
			fluke::ChangeWindowAttributes{conn, win, XCB_CW_EVENT_MASK, &fluke::XCB_WINDOW_EVENTS};
			fluke::ChangeWindowAttributes{conn, win, XCB_CONFIG_WINDOW_BORDER_WIDTH, &constants::BORDER_SIZE};
			fluke::ChangeWindowAttributes{conn, win, XCB_CW_BORDER_PIXEL, &constants::BORDER_COLOUR_INACTIVE};
		}

		// get currently focused window
		xcb_window_t focused = fluke::GetInputFocus{conn}.get()->focus; // get currently focused window

		const uint32_t values[] = { constants::BORDER_SIZE, XCB_STACK_MODE_ABOVE };

		if (focused != conn.root()) {
			fluke::ConfigureWindow{conn, focused, XCB_CONFIG_WINDOW_BORDER_WIDTH | XCB_CONFIG_WINDOW_STACK_MODE, values};
			fluke::ChangeWindowAttributes{conn, focused, XCB_CW_BORDER_PIXEL, &constants::BORDER_COLOUR_ACTIVE};
		}
	}

}

