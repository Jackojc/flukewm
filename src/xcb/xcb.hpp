#pragma once

#include <iomanip>
#include <sstream>
#include <cstdint>

extern "C" {
#include <xcb/xcb.h>
#include <xcb/xcb_aux.h>
#include <xcb/xcb_atom.h>
#include <xcb/xcb_icccm.h>
#include <xcb/xcb_ewmh.h>
#include <xcb/xcb_keysyms.h>
#include <xcb/xcb_event.h>
#include <xcb/xproto.h>
#include <xcb/randr.h>
}


namespace fluke {
	constexpr uint32_t XCB_MOVE_RESIZE =
		XCB_CONFIG_WINDOW_X |
		XCB_CONFIG_WINDOW_Y |
		XCB_CONFIG_WINDOW_WIDTH |
		XCB_CONFIG_WINDOW_HEIGHT
	;

	constexpr uint32_t XCB_MOVE =
		XCB_CONFIG_WINDOW_X |
		XCB_CONFIG_WINDOW_Y
	;

	constexpr uint32_t XCB_RESIZE =
		XCB_CONFIG_WINDOW_WIDTH |
		XCB_CONFIG_WINDOW_HEIGHT
	;

	constexpr uint32_t XCB_WINDOWMANAGER_EVENTS =
		XCB_EVENT_MASK_STRUCTURE_NOTIFY |
		XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY |
		XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT |
		XCB_EVENT_MASK_BUTTON_PRESS
	;

	constexpr uint32_t XCB_WINDOW_EVENTS =
		XCB_EVENT_MASK_ENTER_WINDOW |
		XCB_EVENT_MASK_LEAVE_WINDOW |
		XCB_EVENT_MASK_FOCUS_CHANGE |
		XCB_EVENT_MASK_STRUCTURE_NOTIFY
	;
}
