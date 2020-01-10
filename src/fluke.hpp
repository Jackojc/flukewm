#pragma once

namespace fluke::config {
	// argb format
	constexpr uint32_t BORDER_COLOUR_INACTIVE = 0x66000000;
	constexpr uint32_t BORDER_COLOUR_ACTIVE   = 0x66ffffff;

	constexpr uint32_t BORDER_SIZE = 1;


	// If enabled, windows will become focused when the mouse
	// enters their bounds.
	constexpr bool MOUSE_FOCUS = false;


	// This flag makes it so that if the cursor moves off a window
	// and onto the root window, do not unfocus the window.
	// Otherwise, the focus would be switched to the root window.
	constexpr bool USE_LAZY_FOCUS = true;


	// This will lock the cursor to the bounds of the focused
	// window when true.
	constexpr bool LOCK_CURSOR_TO_WINDOW = true;


	// If true, windows will have borders and title bars.
	constexpr bool REPARENT_WINDOWS = true;
}


// #define TINGE_PLAIN
#include <tinge.hpp>

#include <xcb/xcb.hpp>
#include <xcb/xcb_errors.hpp>

#include <structures/connection.hpp>
#include <structures/request.hpp>

#include <utils/zip.hpp>
#include <utils/exec.hpp>
#include <utils/events.hpp>
#include <utils/keys.hpp>
#include <utils/misc.hpp>

#include <X11/keysym.h>
#include <X11/XF86keysym.h>
#include <actions.hpp>


namespace fluke::config {
	// Keys
	const fluke::Keys keys {
		fluke::Key{ fluke::XCB_MASK_SUPER | fluke::XCB_MASK_SHIFT, XK_Left,  fluke::action_resize, { -10, 0, 0, 0 } },
		fluke::Key{ fluke::XCB_MASK_SUPER | fluke::XCB_MASK_SHIFT, XK_Right, fluke::action_resize, { +10, 0, 0, 0 } },
		fluke::Key{ fluke::XCB_MASK_SUPER | fluke::XCB_MASK_SHIFT, XK_Up,    fluke::action_resize, { 0, -10, 0, 0 } },
		fluke::Key{ fluke::XCB_MASK_SUPER | fluke::XCB_MASK_SHIFT, XK_Down,  fluke::action_resize, { 0, +10, 0, 0 } },

		fluke::Key{ fluke::XCB_MASK_SUPER | fluke::XCB_MASK_ALT, XK_Left,  fluke::action_resize, { -10, 0, +10, 0 } },
		fluke::Key{ fluke::XCB_MASK_SUPER | fluke::XCB_MASK_ALT, XK_Right, fluke::action_resize, { 0, 0, +10, 0 } },
		fluke::Key{ fluke::XCB_MASK_SUPER | fluke::XCB_MASK_ALT, XK_Up,    fluke::action_resize, { 0, -10, 0, +10 } },
		fluke::Key{ fluke::XCB_MASK_SUPER | fluke::XCB_MASK_ALT, XK_Down,  fluke::action_resize, { 0, 0, 0, +10 } },

		fluke::Key{ fluke::XCB_MASK_SUPER | fluke::XCB_MASK_ALT | fluke::XCB_MASK_SHIFT, XK_Left,  fluke::action_resize, { 0, 0, -10, 0 } },
		fluke::Key{ fluke::XCB_MASK_SUPER | fluke::XCB_MASK_ALT | fluke::XCB_MASK_SHIFT, XK_Right, fluke::action_resize, { +10, 0, -10, 0 } },
		fluke::Key{ fluke::XCB_MASK_SUPER | fluke::XCB_MASK_ALT | fluke::XCB_MASK_SHIFT, XK_Up,    fluke::action_resize, { 0, 0, 0, -10 } },
		fluke::Key{ fluke::XCB_MASK_SUPER | fluke::XCB_MASK_ALT | fluke::XCB_MASK_SHIFT, XK_Down,  fluke::action_resize, { 0, +10, 0, -10 } },

		fluke::Key{ fluke::XCB_MASK_SUPER, XK_equal, fluke::action_resize, { -10, -10, +20, +20 } },
		fluke::Key{ fluke::XCB_MASK_SUPER, XK_minus, fluke::action_resize, { +10, +10, -20, -20 } },

		fluke::Key{ fluke::XCB_MASK_ALT, XK_Tab, fluke::action_focus, { FOCUS_NEXT } },
		fluke::Key{ fluke::XCB_MASK_ALT | fluke::XCB_MASK_SHIFT, XK_Tab, fluke::action_focus, { FOCUS_PREV } },
	};
}


#include <user_hooks.hpp>
#include <event_handlers.hpp>

