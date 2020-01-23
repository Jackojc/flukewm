#pragma once


namespace fluke::config {
	// argb format
	constexpr auto BORDER_COLOUR_INACTIVE = 0xff666666;
	constexpr auto BORDER_COLOUR_ACTIVE   = 0xffffffff;

	constexpr auto BORDER_SIZE = 2;


	// Percentage of screen size that new windows should
	// take up.
	constexpr auto NEW_WINDOW_PERCENT = 80;


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

	constexpr auto GAP = 2;
}


