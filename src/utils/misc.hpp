#pragma once

#include <vector>
#include <algorithm>

#include <fluke.hpp>

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

