#pragma once


namespace fluke::config {
	// argb format
	constexpr auto BORDER_COLOUR_INACTIVE = 0xff666666;
	constexpr auto BORDER_COLOUR_ACTIVE   = 0xffffffff;

	constexpr auto BORDER_SIZE = 1;


	// Percentage of screen size that new windows should
	// take up.
	constexpr auto NEW_WINDOW_PERCENT = 65;


	// If true, windows will have borders and title bars.
	constexpr auto REPARENT_WINDOWS = true;


	// For tiling, you can offset a certain number of pixels
	// to reserve space for a dock or statusbar.
	constexpr auto GUTTER_LEFT   = 1;
	constexpr auto GUTTER_RIGHT  = 1;
	constexpr auto GUTTER_TOP    = 1;
	constexpr auto GUTTER_BOTTOM = 1;

	constexpr auto GAP = 1;
}


