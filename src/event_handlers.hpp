#pragma once

#include <utility>

#include <fluke.hpp>

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
