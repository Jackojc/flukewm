#pragma once

#include <vector>
#include <algorithm>

#include <fluke.hpp>

namespace fluke {
	#define FLUKE_DEBUG(x) if constexpr(constants::DEBUG) { x; }





	auto get_all_windows(fluke::Connection& conn) {
		FLUKE_DEBUG( tinge::warnln("get_all_windows") )

		// Get all windows.
		FLUKE_DEBUG( tinge::noticeln(tinge::before{"\t"}, "getting window ids") )

		auto tree = fluke::GetTree{conn, conn.root()}.get();
		std::vector<xcb_window_t> windows{
			xcb_query_tree_children(tree.get()),  // pointer to array of windows.
			xcb_query_tree_children(tree.get()) + xcb_query_tree_children_length(tree.get())
		};

		// Get window attributes.
		FLUKE_DEBUG( tinge::noticeln(tinge::before{"\t"}, "getting window attributes") )
		auto attrs = fluke::RequestContainer<fluke::GetWindowAttributes>{conn, windows}.get();

		// Ignore windows which have override_redirect set.
		FLUKE_DEBUG( tinge::noticeln(tinge::before{"\t"}, "removing ignored windows") )

		decltype(attrs)::size_type i = 0;
		windows.erase(std::remove_if(windows.begin(), windows.end(), [&] (xcb_window_t) {
			return attrs[i++]->override_redirect;
		}), windows.end());

		return windows;
	}






	auto get_mapped_windows(fluke::Connection& conn) {
		FLUKE_DEBUG( tinge::warnln("get_mapped_windows") )

		// Get all windows.
		FLUKE_DEBUG( tinge::noticeln(tinge::before{"\t"}, "getting window ids") )

		auto tree = fluke::GetTree{conn, conn.root()}.get();
		std::vector<xcb_window_t> windows{
			xcb_query_tree_children(tree.get()),  // pointer to array of windows.
			xcb_query_tree_children(tree.get()) + xcb_query_tree_children_length(tree.get())
		};

		// Get window attributes.
		FLUKE_DEBUG( tinge::noticeln(tinge::before{"\t"}, "getting window attributes") )
		auto attrs = fluke::RequestContainer<fluke::GetWindowAttributes>{conn, windows}.get();

		// Ignore windows which have override_redirect set.
		FLUKE_DEBUG( tinge::noticeln(tinge::before{"\t"}, "removing ignored windows") )

		decltype(attrs)::size_type i = 0;
		windows.erase(std::remove_if(windows.begin(), windows.end(), [&] (xcb_window_t) {
			bool flag = not attrs.at(i)->override_redirect and attrs.at(i)->map_state == XCB_MAP_STATE_VIEWABLE;
			i++;
			return not flag;
		}), windows.end());

		return windows;
	}






	void adopt_orphaned_windows(fluke::Connection& conn) {
		auto windows = fluke::get_mapped_windows(conn);

		if (windows.size() == 0)
			return;

		FLUKE_DEBUG( tinge::warnln("adopt_orphaned_windows") )


		// register events on windows.
		FLUKE_DEBUG( tinge::noticeln(tinge::before{"\t"}, "registering events on windows") )
		fluke::RequestContainer<fluke::SetWindowAttributes> request_events{
			conn, windows, XCB_CW_EVENT_MASK, &fluke::XCB_WINDOW_EVENTS
		};


		// Set border width.
		FLUKE_DEBUG( tinge::noticeln(tinge::before{"\t"}, "setting window border sizes") )
		fluke::RequestContainer<fluke::SetWindowConfig> request_borders{
			conn, windows, XCB_CONFIG_WINDOW_BORDER_WIDTH, &constants::BORDER_SIZE
		};


		// Set border colour.
		FLUKE_DEBUG( tinge::noticeln(tinge::before{"\t"}, "setting window border colours") )
		fluke::RequestContainer<fluke::SetWindowAttributes> request_colours{
			conn, windows, XCB_CW_BORDER_PIXEL, &constants::BORDER_COLOUR_INACTIVE
		};


		// commit changes
		fluke::get(request_events, request_colours, request_borders);


		// get currently focused window
		FLUKE_DEBUG( tinge::noticeln(tinge::before{"\t"}, "getting focused window") )
		xcb_window_t focused = fluke::GetInputFocus{conn}.get()->focus; // get currently focused window

		// if (focused != conn.root()) {
		// 	// highlight currently focused window and set stacking order.
		// 	FLUKE_DEBUG( tinge::noticeln(tinge::before{"\t"}, "setting border and stacking mode for focused window") )

		// 	uint32_t values[] = { constants::BORDER_SIZE, XCB_STACK_MODE_ABOVE };
		// 	fluke::RequestBuffer{
		// 		fluke::SetWindowConfig{
		// 			conn, focused, XCB_CONFIG_WINDOW_BORDER_WIDTH | XCB_CONFIG_WINDOW_STACK_MODE, values
		// 		},
		// 		fluke::SetWindowAttributes{
		// 			conn, focused, XCB_CW_BORDER_PIXEL, &constants::BORDER_COLOUR_ACTIVE
		// 		}
		// 	}.get();
		// }
	}

}

