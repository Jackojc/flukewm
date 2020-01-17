#pragma once


// Various settings
namespace fluke::config {
	// argb format
	constexpr auto BORDER_COLOUR_INACTIVE = 0xff666666;
	constexpr auto BORDER_COLOUR_ACTIVE   = 0xffffffff;

	constexpr auto BORDER_SIZE = 1;


	// If enabled, windows will become focused when the mouse
	// enters their bounds.
	constexpr auto MOUSE_FOCUS = false;


	// This flag makes it so that if the cursor moves off a window
	// and onto the root window, do not unfocus the window.
	// Otherwise, the focus would be switched to the root window.
	constexpr auto USE_LAZY_FOCUS = false;


	// This will lock the cursor to the bounds of the focused
	// window when true.
	constexpr auto LOCK_CURSOR_TO_WINDOW = true;


	// If true, windows will have borders and title bars.
	constexpr auto REPARENT_WINDOWS = true;


	// For tiling, you can offset a certain number of pixels
	// to reserve space for a dock or statusbar.
	constexpr auto GUTTER_LEFT   = 0;
	constexpr auto GUTTER_RIGHT  = 0;
	constexpr auto GUTTER_TOP    = 0;
	constexpr auto GUTTER_BOTTOM = 0;

	constexpr auto GAP = 0;
}


// #define TINGE_PLAIN
#include <tinge.hpp>

#include <xcb/xcb.hpp>
#include <xcb/xcb_errors.hpp>

#include <structures/types.hpp>
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


// Keybindings


// Macro to partially apply callback functions with variadic arguments.
#define CALLBACK(func, ...) [] (auto& conn) { func(conn, ##__VA_ARGS__); }

namespace fluke::config {
	constexpr fluke::Keys keys {
		// fluke::Key{ modifier, key, CALLBACK(func, args) }
		fluke::Key{ fluke::XCB_MASK_SUPER | fluke::XCB_MASK_SHIFT, XK_Left,  CALLBACK(fluke::action_resize, -10, 0, 0, 0) },
		fluke::Key{ fluke::XCB_MASK_SUPER | fluke::XCB_MASK_SHIFT, XK_Right, CALLBACK(fluke::action_resize, +10, 0, 0, 0) },
		fluke::Key{ fluke::XCB_MASK_SUPER | fluke::XCB_MASK_SHIFT, XK_Up,    CALLBACK(fluke::action_resize, 0, -10, 0, 0) },
		fluke::Key{ fluke::XCB_MASK_SUPER | fluke::XCB_MASK_SHIFT, XK_Down,  CALLBACK(fluke::action_resize, 0, +10, 0, 0) },

		fluke::Key{ fluke::XCB_MASK_SUPER | fluke::XCB_MASK_ALT, XK_Left,  CALLBACK(fluke::action_resize, -10, 0, +10, 0) },
		fluke::Key{ fluke::XCB_MASK_SUPER | fluke::XCB_MASK_ALT, XK_Right, CALLBACK(fluke::action_resize, 0, 0, +10, 0) },
		fluke::Key{ fluke::XCB_MASK_SUPER | fluke::XCB_MASK_ALT, XK_Up,    CALLBACK(fluke::action_resize, 0, -10, 0, +10) },
		fluke::Key{ fluke::XCB_MASK_SUPER | fluke::XCB_MASK_ALT, XK_Down,  CALLBACK(fluke::action_resize, 0, 0, 0, +10) },

		fluke::Key{ fluke::XCB_MASK_SUPER | fluke::XCB_MASK_ALT | fluke::XCB_MASK_SHIFT, XK_Left,  CALLBACK(fluke::action_resize, 0, 0, -10, 0) },
		fluke::Key{ fluke::XCB_MASK_SUPER | fluke::XCB_MASK_ALT | fluke::XCB_MASK_SHIFT, XK_Right, CALLBACK(fluke::action_resize, +10, 0, -10, 0) },
		fluke::Key{ fluke::XCB_MASK_SUPER | fluke::XCB_MASK_ALT | fluke::XCB_MASK_SHIFT, XK_Up,    CALLBACK(fluke::action_resize, 0, 0, 0, -10) },
		fluke::Key{ fluke::XCB_MASK_SUPER | fluke::XCB_MASK_ALT | fluke::XCB_MASK_SHIFT, XK_Down,  CALLBACK(fluke::action_resize, 0, +10, 0, -10) },

		fluke::Key{ fluke::XCB_MASK_SUPER, XK_equal, CALLBACK(fluke::action_resize, -10, -10, +20, +20) },
		fluke::Key{ fluke::XCB_MASK_SUPER, XK_minus, CALLBACK(fluke::action_resize, +10, +10, -20, -20) },

		fluke::Key{ fluke::XCB_MASK_ALT,                         XK_Tab, CALLBACK(fluke::action_focus, FOCUS_NEXT) },
		fluke::Key{ fluke::XCB_MASK_ALT | fluke::XCB_MASK_SHIFT, XK_Tab, CALLBACK(fluke::action_focus, FOCUS_PREV) },

		fluke::Key{ fluke::XCB_MASK_SUPER, XK_Left,  CALLBACK(fluke::action_focus_dir, FOCUS_LEFT) },
		fluke::Key{ fluke::XCB_MASK_SUPER, XK_Right, CALLBACK(fluke::action_focus_dir, FOCUS_RIGHT) },
		fluke::Key{ fluke::XCB_MASK_SUPER, XK_Up,    CALLBACK(fluke::action_focus_dir, FOCUS_UP) },
		fluke::Key{ fluke::XCB_MASK_SUPER, XK_Down,  CALLBACK(fluke::action_focus_dir, FOCUS_DOWN) },

		fluke::Key{ fluke::XCB_MASK_SUPER, XK_t, CALLBACK(fluke::action_layout_masterslave, MASTER_LEFT) },
	};
}

#undef CALLBACK


// User hooks
namespace fluke {
	// These hooks are called when window focus is changed.
	inline void on_focus_in(fluke::Connection& conn) {}
	inline void on_focus_out(fluke::Connection& conn) {}


	// These hooks are called when the mouse enters/leaves a window.
	inline void on_hover_in(fluke::Connection& conn) {}
	inline void on_hover_out(fluke::Connection& conn) {}


	// Called on window creation or destruction.
	inline void on_create(fluke::Connection& conn) {}
	inline void on_destroy(fluke::Connection& conn) {}


	// Called when windows are mapped or unmapped.
	inline void on_map(fluke::Connection& conn) {}
	inline void on_unmap(fluke::Connection& conn) {}


	// This is called when a window asks to be configured.
	inline void on_configure(fluke::Connection& conn) {}


	// This is called when a grabbed key is pressed.
	inline void on_keypress(fluke::Connection& conn) {}


	// Called when an error occurs.
	inline void on_error(fluke::Connection& conn) {}


	// These are usually called when anything EWMH happens.
	inline void on_property(fluke::Connection& conn) {}
	inline void on_client_message(fluke::Connection& conn) {}


	// Randr/screen hooks.
	inline void on_randr_screen_change(fluke::Connection& conn) {}
	inline void on_randr_notify(fluke::Connection& conn) {}
}


#include <event_handlers.hpp>

