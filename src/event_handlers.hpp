#pragma once

#include <utility>

#include <fluke.hpp>

namespace fluke::event_handlers {
	// void(*)(fluke::Connection&, fluke::Event&&);

	inline void event_enter_notify(fluke::Connection& conn, fluke::Event&& e_) {
		auto e = fluke::event_cast<fluke::EnterNotifyEvent>(std::move(e_));
		xcb_window_t win = e->event;

		FLUKE_DEBUG( tinge::successln(fluke::to_hex(win), " ENTER_NOTIFY") )

		if (e->mode != XCB_NOTIFY_MODE_NORMAL and e->mode != XCB_NOTIFY_MODE_UNGRAB)
			return;

		// set input focus to new window, set borders and stacking order.
		FLUKE_DEBUG( tinge::noticeln(tinge::before{"\t"}, "setting input focus and stacking mode") )

		fluke::SetInputFocus{conn, XCB_INPUT_FOCUS_POINTER_ROOT, win, XCB_CURRENT_TIME};
		fluke::ConfigureWindow{conn, win, XCB_CONFIG_WINDOW_STACK_MODE, fluke::data{XCB_STACK_MODE_ABOVE}};
	}



	inline void event_leave_notify(fluke::Connection& conn, fluke::Event&& e_) {
		auto e = fluke::event_cast<fluke::LeaveNotifyEvent>(std::move(e_));
		xcb_window_t win = e->event;

		FLUKE_DEBUG( tinge::successln(fluke::to_hex(win), " LEAVE_NOTIFY") )

		fluke::SetInputFocus{conn, XCB_INPUT_FOCUS_POINTER_ROOT, conn.root(), XCB_CURRENT_TIME};
	}



	inline void event_focus_in(fluke::Connection& conn, fluke::Event&& e_) {
		auto e = fluke::event_cast<fluke::FocusInEvent>(std::move(e_));
		xcb_window_t win = e->event;

		FLUKE_DEBUG( tinge::successln(fluke::to_hex(win), " FOCUS_IN") )

		fluke::ConfigureWindow{conn, win, XCB_CONFIG_WINDOW_BORDER_WIDTH, &constants::BORDER_SIZE};
		fluke::ChangeWindowAttributes{conn, win, XCB_CW_BORDER_PIXEL, &constants::BORDER_COLOUR_ACTIVE};
	}



	inline void event_focus_out(fluke::Connection& conn, fluke::Event&& e_) {
		auto e = fluke::event_cast<fluke::FocusOutEvent>(std::move(e_));
		xcb_window_t win = e->event;

		FLUKE_DEBUG( tinge::successln(fluke::to_hex(win), " FOCUS_OUT") )

		fluke::ConfigureWindow{conn, win, XCB_CONFIG_WINDOW_BORDER_WIDTH, &constants::BORDER_SIZE};
		fluke::ChangeWindowAttributes{conn, win, XCB_CW_BORDER_PIXEL, &constants::BORDER_COLOUR_INACTIVE};
	}





	inline void event_create_notify(fluke::Connection& conn, fluke::Event&& e_) {
		auto e = fluke::event_cast<fluke::CreateNotifyEvent>(std::move(e_));
		xcb_window_t win = e->window;

		FLUKE_DEBUG( tinge::successln(fluke::to_hex(win), " CREATE_NOTIFY") )

		if (fluke::GetWindowAttributes{conn, win}.get()->override_redirect)
			return;

		auto&& [cursor, geom] = fluke::RequestBuffer{
			fluke::QueryPointer{conn, conn.root()},
			fluke::GetGeometry{conn, win}
		}.get();

		auto cursor_x = cursor->root_x;
		auto cursor_y = cursor->root_y;
		auto w = geom->width;
		auto h = geom->height;

		uint32_t x = static_cast<uint32_t>(cursor_x) - static_cast<uint32_t>(w) / 2;
		uint32_t y = static_cast<uint32_t>(cursor_y) - static_cast<uint32_t>(h) / 2;

		uint32_t values[] = { x, y, w, h };

		fluke::ConfigureWindow{conn, win, fluke::XCB_MOVE_RESIZE, values};
		fluke::ChangeWindowAttributes{conn, win, XCB_CW_EVENT_MASK, &fluke::XCB_WINDOW_EVENTS};
	}



	inline void event_map_request(fluke::Connection& conn, fluke::Event&& e_) {
		auto e = fluke::event_cast<fluke::MapRequestEvent>(std::move(e_));
		xcb_window_t win = e->window;

		FLUKE_DEBUG( tinge::successln(fluke::to_hex(win), " MAP_REQUEST") )

		if (fluke::GetWindowAttributes{conn, win}.get()->override_redirect)
			return;

		fluke::MapWindow{conn, win};
		fluke::ConfigureWindow{conn, win, XCB_CONFIG_WINDOW_BORDER_WIDTH, &constants::BORDER_SIZE};
		fluke::ChangeWindowAttributes{conn, win, XCB_CW_BORDER_PIXEL, &constants::BORDER_COLOUR_ACTIVE};
		fluke::SetInputFocus{conn, XCB_INPUT_FOCUS_POINTER_ROOT, win, XCB_CURRENT_TIME};
	}


	// this event cant be very hot if using a tool like xmmv
	inline void event_configure_request(fluke::Connection& conn, fluke::Event&& e_) {
		auto e = fluke::event_cast<fluke::ConfigureRequestEvent>(std::move(e_));
		xcb_window_t win = e->window;
		uint16_t mask = e->value_mask;

		FLUKE_DEBUG( tinge::successln(fluke::to_hex(win), " CONFIGURE_REQUEST") )

		// this functions essentially as a stack to push values onto.
		uint8_t i = 0;
		std::array<uint32_t, 5> values{};

		if (mask & XCB_CONFIG_WINDOW_X)          values[i++] = static_cast<uint32_t>(e->x);
		if (mask & XCB_CONFIG_WINDOW_Y)          values[i++] = static_cast<uint32_t>(e->y);
		if (mask & XCB_CONFIG_WINDOW_WIDTH)      values[i++] = e->width;
		if (mask & XCB_CONFIG_WINDOW_HEIGHT)     values[i++] = e->height;
		if (mask & XCB_CONFIG_WINDOW_STACK_MODE) values[i++] = e->stack_mode;

		fluke::ConfigureWindow{conn, win, mask, values.data()};
	}
}

