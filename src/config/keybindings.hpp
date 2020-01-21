#pragma once


// Keybindings

// Macro to partially apply callback functions with variadic arguments.
#define ACTION(func, ...) [] (auto& conn) { func(conn, ##__VA_ARGS__); }
#define RUN(...) [] (auto&) { fluke::exec(__VA_ARGS__); }

namespace fluke::config {
	constexpr fluke::Keys keys {
		// fluke::Key{ modifier, key, ACTION(func, args) }
		fluke::Key{ fluke::XCB_MASK_SUPER | fluke::XCB_MASK_SHIFT, XK_Left,  ACTION(fluke::action_resize, -10, 0, 0, 0) },
		fluke::Key{ fluke::XCB_MASK_SUPER | fluke::XCB_MASK_SHIFT, XK_Right, ACTION(fluke::action_resize, +10, 0, 0, 0) },
		fluke::Key{ fluke::XCB_MASK_SUPER | fluke::XCB_MASK_SHIFT, XK_Up,    ACTION(fluke::action_resize, 0, -10, 0, 0) },
		fluke::Key{ fluke::XCB_MASK_SUPER | fluke::XCB_MASK_SHIFT, XK_Down,  ACTION(fluke::action_resize, 0, +10, 0, 0) },

		fluke::Key{ fluke::XCB_MASK_SUPER | fluke::XCB_MASK_ALT, XK_Left,  ACTION(fluke::action_resize, -10, 0, +10, 0) },
		fluke::Key{ fluke::XCB_MASK_SUPER | fluke::XCB_MASK_ALT, XK_Right, ACTION(fluke::action_resize, 0, 0, +10, 0) },
		fluke::Key{ fluke::XCB_MASK_SUPER | fluke::XCB_MASK_ALT, XK_Up,    ACTION(fluke::action_resize, 0, -10, 0, +10) },
		fluke::Key{ fluke::XCB_MASK_SUPER | fluke::XCB_MASK_ALT, XK_Down,  ACTION(fluke::action_resize, 0, 0, 0, +10) },

		fluke::Key{ fluke::XCB_MASK_SUPER | fluke::XCB_MASK_ALT | fluke::XCB_MASK_SHIFT, XK_Left,  ACTION(fluke::action_resize, 0, 0, -10, 0) },
		fluke::Key{ fluke::XCB_MASK_SUPER | fluke::XCB_MASK_ALT | fluke::XCB_MASK_SHIFT, XK_Right, ACTION(fluke::action_resize, +10, 0, -10, 0) },
		fluke::Key{ fluke::XCB_MASK_SUPER | fluke::XCB_MASK_ALT | fluke::XCB_MASK_SHIFT, XK_Up,    ACTION(fluke::action_resize, 0, 0, 0, -10) },
		fluke::Key{ fluke::XCB_MASK_SUPER | fluke::XCB_MASK_ALT | fluke::XCB_MASK_SHIFT, XK_Down,  ACTION(fluke::action_resize, 0, +10, 0, -10) },

		fluke::Key{ fluke::XCB_MASK_SUPER, XK_equal, ACTION(fluke::action_resize, -10, -10, +20, +20) },
		fluke::Key{ fluke::XCB_MASK_SUPER, XK_minus, ACTION(fluke::action_resize, +10, +10, -20, -20) },

		fluke::Key{ fluke::XCB_MASK_ALT,                         XK_Tab, ACTION(fluke::action_focus, FOCUS_NEXT) },
		fluke::Key{ fluke::XCB_MASK_ALT | fluke::XCB_MASK_SHIFT, XK_Tab, ACTION(fluke::action_focus, FOCUS_PREV) },

		fluke::Key{ fluke::XCB_MASK_SUPER, XK_Left,  ACTION(fluke::action_focus_dir, FOCUS_LEFT) },
		fluke::Key{ fluke::XCB_MASK_SUPER, XK_Right, ACTION(fluke::action_focus_dir, FOCUS_RIGHT) },
		fluke::Key{ fluke::XCB_MASK_SUPER, XK_Up,    ACTION(fluke::action_focus_dir, FOCUS_UP) },
		fluke::Key{ fluke::XCB_MASK_SUPER, XK_Down,  ACTION(fluke::action_focus_dir, FOCUS_DOWN) },

		fluke::Key{ fluke::XCB_MASK_SUPER | fluke::XCB_MASK_CONTROL, XK_Left,  ACTION(fluke::action_snap_side, SNAP_LEFT) },
		fluke::Key{ fluke::XCB_MASK_SUPER | fluke::XCB_MASK_CONTROL, XK_Right,  ACTION(fluke::action_snap_side, SNAP_RIGHT) },
		fluke::Key{ fluke::XCB_MASK_SUPER | fluke::XCB_MASK_CONTROL, XK_Up,  ACTION(fluke::action_snap_side, SNAP_TOP) },
		fluke::Key{ fluke::XCB_MASK_SUPER | fluke::XCB_MASK_CONTROL, XK_Down,  ACTION(fluke::action_snap_side, SNAP_BOTTOM) },

		fluke::Key{ fluke::XCB_MASK_SUPER, XK_t, ACTION(fluke::action_layout_masterslave, MASTER_LEFT) },

		fluke::Key{ fluke::XCB_MASK_SUPER, XK_c, ACTION(fluke::action_center) },



		// Programs.
		fluke::Key{ fluke::XCB_MASK_SUPER, XK_Return, RUN("st") },
		fluke::Key{ fluke::XCB_MASK_SUPER, XK_Tab, RUN("dmenu_run", "-i") },
		fluke::Key{ fluke::XCB_MASK_SUPER, XK_w, RUN("wallpaper_random") },
	};
}

#undef ACTION
#undef RUN

