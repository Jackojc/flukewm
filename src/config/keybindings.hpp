#pragma once


// Keybindings

// Macros to partially apply callback functions with variadic arguments.
#define ACTION(func, ...) [] (auto& conn) { func(conn, ##__VA_ARGS__); }
#define RUN(...) [] (auto&) { fluke::exec(__VA_ARGS__); }

namespace fluke::config {
	constexpr fluke::Keys keybindings {
		// Move.
		fluke::Key{ fluke::keys::super | fluke::keys::shift, fluke::keys::left,  ACTION(fluke::action_resize, -10, 0, 0, 0) },
		fluke::Key{ fluke::keys::super | fluke::keys::shift, fluke::keys::right, ACTION(fluke::action_resize, +10, 0, 0, 0) },
		fluke::Key{ fluke::keys::super | fluke::keys::shift, fluke::keys::up,    ACTION(fluke::action_resize, 0, -10, 0, 0) },
		fluke::Key{ fluke::keys::super | fluke::keys::shift, fluke::keys::down,  ACTION(fluke::action_resize, 0, +10, 0, 0) },

		// Grow.
		fluke::Key{ fluke::keys::super | fluke::keys::alt, fluke::keys::left,  ACTION(fluke::action_resize, -10, 0, +10, 0) },
		fluke::Key{ fluke::keys::super | fluke::keys::alt, fluke::keys::right, ACTION(fluke::action_resize, 0, 0, +10, 0) },
		fluke::Key{ fluke::keys::super | fluke::keys::alt, fluke::keys::up,    ACTION(fluke::action_resize, 0, -10, 0, +10) },
		fluke::Key{ fluke::keys::super | fluke::keys::alt, fluke::keys::down,  ACTION(fluke::action_resize, 0, 0, 0, +10) },

		// Shrink.
		fluke::Key{ fluke::keys::super | fluke::keys::alt | fluke::keys::shift, fluke::keys::left,  ACTION(fluke::action_resize, 0, 0, -10, 0) },
		fluke::Key{ fluke::keys::super | fluke::keys::alt | fluke::keys::shift, fluke::keys::right, ACTION(fluke::action_resize, +10, 0, -10, 0) },
		fluke::Key{ fluke::keys::super | fluke::keys::alt | fluke::keys::shift, fluke::keys::up,    ACTION(fluke::action_resize, 0, 0, 0, -10) },
		fluke::Key{ fluke::keys::super | fluke::keys::alt | fluke::keys::shift, fluke::keys::down,  ACTION(fluke::action_resize, 0, +10, 0, -10) },

		// Grow or shrink equally on all sides simultaneously.
		fluke::Key{ fluke::keys::super, fluke::keys::equal, ACTION(fluke::action_resize, -10, -10, +20, +20) },
		fluke::Key{ fluke::keys::super, fluke::keys::minus, ACTION(fluke::action_resize, +10, +10, -20, -20) },

		// Focus switching.
		fluke::Key{ fluke::keys::alt, fluke::keys::tab, ACTION(fluke::action_focus, FOCUS_NEXT) },
		fluke::Key{ fluke::keys::alt | fluke::keys::shift, fluke::keys::tab, ACTION(fluke::action_focus, FOCUS_PREV) },

		// Directional focusing.
		fluke::Key{ fluke::keys::super, fluke::keys::left,  ACTION(fluke::action_focus_dir, FOCUS_LEFT) },
		fluke::Key{ fluke::keys::super, fluke::keys::right, ACTION(fluke::action_focus_dir, FOCUS_RIGHT) },
		fluke::Key{ fluke::keys::super, fluke::keys::up,    ACTION(fluke::action_focus_dir, FOCUS_UP) },
		fluke::Key{ fluke::keys::super, fluke::keys::down,  ACTION(fluke::action_focus_dir, FOCUS_DOWN) },

		// Snap to sides.
		fluke::Key{ fluke::keys::super | fluke::keys::control, fluke::keys::left,  ACTION(fluke::action_snap_side, SNAP_LEFT) },
		fluke::Key{ fluke::keys::super | fluke::keys::control, fluke::keys::right,  ACTION(fluke::action_snap_side, SNAP_RIGHT) },
		fluke::Key{ fluke::keys::super | fluke::keys::control, fluke::keys::up,  ACTION(fluke::action_snap_side, SNAP_TOP) },
		fluke::Key{ fluke::keys::super | fluke::keys::control, fluke::keys::down,  ACTION(fluke::action_snap_side, SNAP_BOTTOM) },

		// Tiling & Misc.
		fluke::Key{ fluke::keys::super, fluke::keys::t, ACTION(fluke::action_layout_masterslave, MASTER_LEFT) },
		fluke::Key{ fluke::keys::super, fluke::keys::c, ACTION(fluke::action_center) },

		// Launch programs.
		fluke::Key{ fluke::keys::super, fluke::keys::ret, RUN("st") },
		fluke::Key{ fluke::keys::super, fluke::keys::tab, RUN("dmenu_run", "-i") },
		fluke::Key{ fluke::keys::super, fluke::keys::w, RUN("wallpaper_random") },
	};
}

#undef ACTION
#undef RUN

