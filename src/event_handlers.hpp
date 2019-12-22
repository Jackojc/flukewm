#pragma once

#include <utility>

#include <fluke.hpp>

namespace fluke::event_handlers {
	// void(*)(fluke::Connection&, fluke::Event&&);

	void event_enter(fluke::Connection& conn, fluke::Event&& e_) {
		auto e = fluke::event_cast<fluke::EnterNotifyEvent>(std::move(e_));
		xcb_window_t win = e->event;

		FLUKE_DEBUG( tinge::successln("enter: ", fluke::to_hex(win)) )

		// respect ignored windows
		if (fluke::GetWindowAttributes{conn, win}.get()->override_redirect) {
			FLUKE_DEBUG( tinge::warnln(tinge::before{"\t"}, "ignored") )
			return;
		}

		// set input focus to new window, set borders and stacking order.
		FLUKE_DEBUG( tinge::noticeln(tinge::before{"\t"}, "setting input focus and stacking mode") )
		fluke::RequestBuffer{
			fluke::SetInputFocus{conn, XCB_INPUT_FOCUS_NONE, win, XCB_CURRENT_TIME},
			fluke::SetWindowConfig{conn, win, XCB_CONFIG_WINDOW_STACK_MODE, fluke::data{XCB_STACK_MODE_ABOVE}}
		}.get();
	}



	void event_leave(fluke::Connection& conn, fluke::Event&& e_) {
		auto e = fluke::event_cast<fluke::LeaveNotifyEvent>(std::move(e_));
		xcb_window_t win = e->event;

		FLUKE_DEBUG( tinge::successln("leave: ", fluke::to_hex(win)) )

		// respect ignored windows
		if (fluke::GetWindowAttributes{conn, win}.get()->override_redirect) {
			FLUKE_DEBUG( tinge::warnln(tinge::before{"\t"}, "ignored") )
			return;
		}

		// set input focus, set borders
		FLUKE_DEBUG( tinge::noticeln(tinge::before{"\t"}, "removing input focus") )
		fluke::SetInputFocus{conn, XCB_INPUT_FOCUS_NONE, XCB_NONE, XCB_CURRENT_TIME}.get();
	}



	void event_focus_in(fluke::Connection& conn, fluke::Event&& e_) {
		auto e = fluke::event_cast<fluke::FocusInEvent>(std::move(e_));
		xcb_window_t win = e->event;

		FLUKE_DEBUG( tinge::successln("focus_in: ", fluke::to_hex(win)) )

		// respect ignored windows
		if (fluke::GetWindowAttributes{conn, win}.get()->override_redirect) {
			FLUKE_DEBUG( tinge::warnln(tinge::before{"\t"}, "ignored") )
			return;
		}

		// set borders
		FLUKE_DEBUG( tinge::noticeln(tinge::before{"\t"}, "setting border width and border colour") )
		fluke::RequestBuffer{
			fluke::SetWindowConfig{conn, win, XCB_CONFIG_WINDOW_BORDER_WIDTH, &constants::BORDER_SIZE},
			fluke::SetWindowAttributes{conn, win, XCB_CW_BORDER_PIXEL, &constants::BORDER_COLOUR_ACTIVE}
		}.get();
	}



	void event_focus_out(fluke::Connection& conn, fluke::Event&& e_) {
		auto e = fluke::event_cast<fluke::FocusOutEvent>(std::move(e_));
		xcb_window_t win = e->event;

		FLUKE_DEBUG( tinge::successln("focus_out: ", fluke::to_hex(win)) )

		if (fluke::GetWindowAttributes{conn, win}.get()->override_redirect) {
			FLUKE_DEBUG( tinge::warnln(tinge::before{"\t"}, "ignored") )
			return;
		}

		// set borders.
		FLUKE_DEBUG( tinge::noticeln(tinge::before{"\t"}, "setting border width and border colour") )
		fluke::RequestBuffer{
			fluke::SetWindowConfig{conn, win, XCB_CONFIG_WINDOW_BORDER_WIDTH, &constants::BORDER_SIZE},
			fluke::SetWindowAttributes{conn, win, XCB_CW_BORDER_PIXEL, &constants::BORDER_COLOUR_INACTIVE}
		}.get();
	}





	void event_create(fluke::Connection& conn, fluke::Event&& e_) {
		auto e = fluke::event_cast<fluke::CreateNotifyEvent>(std::move(e_));
		xcb_window_t win = e->window;

		FLUKE_DEBUG( tinge::successln("create: ", fluke::to_hex(win)) )

		if (fluke::GetWindowAttributes{conn, win}.get()->override_redirect) {
			FLUKE_DEBUG( tinge::warnln(tinge::before{"\t"}, "ignored") )
			return;
		}

		FLUKE_DEBUG( tinge::noticeln(tinge::before{"\t"}, "registering events") )
		fluke::SetWindowAttributes{conn, win, XCB_CW_EVENT_MASK, &fluke::XCB_WINDOW_EVENTS}.get();
	}



	void event_map(fluke::Connection& conn, fluke::Event&& e_) {
		auto e = fluke::event_cast<fluke::MapNotifyEvent>(std::move(e_));
		xcb_window_t win = e->event;

		FLUKE_DEBUG( tinge::successln("map: ", fluke::to_hex(win)) )

		if (fluke::GetWindowAttributes{conn, win}.get()->override_redirect) {
			FLUKE_DEBUG( tinge::warnln(tinge::before{"\t"}, "ignored") )
			return;
		}

		FLUKE_DEBUG( tinge::noticeln(tinge::before{"\t"}, "mapping window") )
		fluke::SetWindowMapped{conn, win}.get();
	}



	void event_configure(fluke::Connection& conn, fluke::Event&& e_) {
		auto e = fluke::event_cast<fluke::ConfigureRequestEvent>(std::move(e_));
		xcb_window_t win = e->window;

		FLUKE_DEBUG( tinge::successln("configure: ", fluke::to_hex(win)) )

		if (fluke::GetWindowAttributes{conn, win}.get()->override_redirect) {
			FLUKE_DEBUG( tinge::warnln(tinge::before{"\t"}, "ignored") )
			return;
		}

		// config border width
		FLUKE_DEBUG( tinge::noticeln(tinge::before{"\t"}, "setting border width") )
		if (e->value_mask & XCB_CONFIG_WINDOW_BORDER_WIDTH)
			fluke::SetWindowConfig{conn, win, XCB_CONFIG_WINDOW_BORDER_WIDTH, &constants::BORDER_SIZE}.get();

		// config window stacking mode
		FLUKE_DEBUG( tinge::noticeln(tinge::before{"\t"}, "setting stacking mode") )
		if (e->value_mask & XCB_CONFIG_WINDOW_STACK_MODE)
			fluke::SetWindowConfig{conn, win, XCB_CONFIG_WINDOW_STACK_MODE, &e->stack_mode}.get();

		// config window rect
		FLUKE_DEBUG( tinge::noticeln(tinge::before{"\t"}, "setting window rect") )
		uint32_t values[] = { static_cast<uint32_t>(e->x), static_cast<uint32_t>(e->y), e->width, e->height };
		fluke::SetWindowAttributes{conn, win, fluke::XCB_MOVE_RESIZE, values}.get();
	}
}

