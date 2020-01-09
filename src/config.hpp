#pragma once

#include <cstdint>
// #include <fluke.hpp>
#include <X11/keysym.h>
#include <X11/XF86keysym.h>


namespace fluke::config {
	// argb format
	constexpr uint32_t BORDER_COLOUR_INACTIVE = 0x66000000;
	constexpr uint32_t BORDER_COLOUR_ACTIVE   = 0x66ffffff;

	constexpr uint32_t BORDER_SIZE = 1;


	// If enabled, windows will become focused when the mouse
	// enters their bounds.
	constexpr bool MOUSE_FOCUS = true;


	// This flag makes it so that if the cursor moves off a window
	// and onto the root window, do not unfocus the window.
	// Otherwise, the focus would be switched to the root window.
	constexpr bool USE_LAZY_FOCUS = true;


	// This will lock the cursor to the bounds of the focused
	// window when true.
	constexpr bool LOCK_CURSOR_TO_WINDOW = true;


	// If true, windows will have borders and title bars.
	constexpr bool REPARENT_WINDOWS = true;




	// Callbacks for keybindings.
	void print_p(fluke::Connection& conn, const std::vector<uint32_t>& arg) {
		tinge::println("key callback `print_p` with arg ");
	}

	void print_a(fluke::Connection& conn, const std::vector<uint32_t>& arg) {
		tinge::println("key callback `print_shift_a` with arg ");
	}



	// Keys
	const fluke::Keys keys {
		fluke::Key{ fluke::XCB_MASK_SUPER, XK_p, print_p, {1333} },
		fluke::Key{ fluke::XCB_MASK_SUPER | fluke::XCB_MASK_SHIFT, XK_a, print_a, {13712} },
	};
}

