#pragma once


// Macros to partially apply callback functions with variadic arguments.
#define ACTION(func, ...) [] (auto& conn) { func(conn, ##__VA_ARGS__); }
#define RUN(...) [] (auto&) { fluke::exec(__VA_ARGS__); }


// Keybindings
namespace fluke::config {
	namespace keys = fluke::keys;

	constexpr fluke::Keys keybindings {
		// Move.
		fluke::Key{ keys::super | keys::shift, keys::left,  ACTION(fluke::action_resize, -30, 0, 0, 0) },
		fluke::Key{ keys::super | keys::shift, keys::right, ACTION(fluke::action_resize, +30, 0, 0, 0) },
		fluke::Key{ keys::super | keys::shift, keys::up,    ACTION(fluke::action_resize, 0, -30, 0, 0) },
		fluke::Key{ keys::super | keys::shift, keys::down,  ACTION(fluke::action_resize, 0, +30, 0, 0) },

		// Grow.
		fluke::Key{ keys::super | keys::alt, keys::left,  ACTION(fluke::action_resize, -20, 0, +20, 0) },
		fluke::Key{ keys::super | keys::alt, keys::right, ACTION(fluke::action_resize, 0, 0, +20, 0) },
		fluke::Key{ keys::super | keys::alt, keys::up,    ACTION(fluke::action_resize, 0, -20, 0, +20) },
		fluke::Key{ keys::super | keys::alt, keys::down,  ACTION(fluke::action_resize, 0, 0, 0, +20) },

		// Shrink.
		fluke::Key{ keys::super | keys::alt | keys::shift, keys::left,  ACTION(fluke::action_resize, 0, 0, -20, 0) },
		fluke::Key{ keys::super | keys::alt | keys::shift, keys::right, ACTION(fluke::action_resize, +20, 0, -20, 0) },
		fluke::Key{ keys::super | keys::alt | keys::shift, keys::up,    ACTION(fluke::action_resize, 0, 0, 0, -20) },
		fluke::Key{ keys::super | keys::alt | keys::shift, keys::down,  ACTION(fluke::action_resize, 0, +20, 0, -20) },

		// Grow or shrink equally on all sides simultaneously.
		fluke::Key{ keys::super, keys::equal, ACTION(fluke::action_resize, -20, -20, +40, +40) },
		fluke::Key{ keys::super, keys::minus, ACTION(fluke::action_resize, +20, +20, -40, -40) },

		// Focus switching.
		fluke::Key{ keys::alt, keys::tab,               ACTION(fluke::action_focus, FOCUS_NEXT) },
		fluke::Key{ keys::alt | keys::shift, keys::tab, ACTION(fluke::action_focus, FOCUS_PREV) },

		// Directional focusing.
		fluke::Key{ keys::super, keys::left,  ACTION(fluke::action_focus_dir, FOCUS_LEFT) },
		fluke::Key{ keys::super, keys::right, ACTION(fluke::action_focus_dir, FOCUS_RIGHT) },
		fluke::Key{ keys::super, keys::up,    ACTION(fluke::action_focus_dir, FOCUS_UP) },
		fluke::Key{ keys::super, keys::down,  ACTION(fluke::action_focus_dir, FOCUS_DOWN) },

		// Snap to sides.
		fluke::Key{ keys::super | keys::control, keys::left,  ACTION(fluke::action_snap, SNAP_SIDE_LEFT) },
		fluke::Key{ keys::super | keys::control, keys::right, ACTION(fluke::action_snap, SNAP_SIDE_RIGHT) },
		fluke::Key{ keys::super | keys::control, keys::up,    ACTION(fluke::action_snap, SNAP_SIDE_TOP) },
		fluke::Key{ keys::super | keys::control, keys::down,  ACTION(fluke::action_snap, SNAP_SIDE_BOTTOM) },

		// Snap to corners.
		fluke::Key{ keys::super | keys::control | keys::shift, keys::left,  ACTION(fluke::action_snap, SNAP_CORNER_TOPLEFT) },
		fluke::Key{ keys::super | keys::control | keys::shift, keys::right, ACTION(fluke::action_snap, SNAP_CORNER_TOPRIGHT) },
		fluke::Key{ keys::super | keys::control | keys::shift, keys::up,    ACTION(fluke::action_snap, SNAP_CORNER_BOTTOMRIGHT) },
		fluke::Key{ keys::super | keys::control | keys::shift, keys::down,  ACTION(fluke::action_snap, SNAP_CORNER_BOTTOMLEFT) },

		// Monitor focus.
		fluke::Key{ keys::super | keys::alt, keys::number_1, ACTION(fluke::action_focus_display_index, 0) },
		fluke::Key{ keys::super | keys::alt, keys::number_2, ACTION(fluke::action_focus_display_index, 1) },
		fluke::Key{ keys::super | keys::alt, keys::number_3, ACTION(fluke::action_focus_display_index, 2) },
		fluke::Key{ keys::super | keys::alt, keys::number_4, ACTION(fluke::action_focus_display_index, 3) },
		fluke::Key{ keys::super | keys::alt, keys::number_5, ACTION(fluke::action_focus_display_index, 4) },
		fluke::Key{ keys::super | keys::alt, keys::number_6, ACTION(fluke::action_focus_display_index, 5) },
		fluke::Key{ keys::super | keys::alt, keys::number_7, ACTION(fluke::action_focus_display_index, 6) },
		fluke::Key{ keys::super | keys::alt, keys::number_8, ACTION(fluke::action_focus_display_index, 7) },
		fluke::Key{ keys::super | keys::alt, keys::number_9, ACTION(fluke::action_focus_display_index, 8) },

		// Tiling & Misc.
		fluke::Key{ keys::super, keys::t, ACTION(fluke::action_layout_masterslave, MASTER_LEFT, 60) },
		fluke::Key{ keys::super | keys::shift, keys::t, ACTION(fluke::action_layout_masterslave, MASTER_RIGHT, 60) },
		fluke::Key{ keys::super, keys::c, ACTION(fluke::action_center_resize) },

		// Launch programs.
		fluke::Key{ keys::super, keys::ret, RUN("st") },
		fluke::Key{ keys::super, keys::tab, RUN("dmenu_run", "-i") },
		fluke::Key{ keys::super, keys::w,   RUN("wallpaper_random") },
		fluke::Key{ keys::super, keys::m,   RUN("xmmv") },
		fluke::Key{ keys::super, keys::r,   RUN("xmrs") },
	};
}


#undef ACTION
#undef RUN

