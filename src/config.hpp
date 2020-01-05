#pragma once

#include <cstdint>
// #include <fluke.hpp>
#include <X11/keysym.h>

namespace fluke::config {
	// argb format
	constexpr uint32_t BORDER_COLOUR_INACTIVE = 0x66000000;
	constexpr uint32_t BORDER_COLOUR_ACTIVE   = 0xffffffff;

	constexpr uint32_t BORDER_SIZE = 2;

	// This flag makes it so that if the cursor moves off a window
	// and onto the root window, do not unfocus the window.
	// Otherwise, the focus would be switched to the root window.
	constexpr bool USE_LAZY_FOCUS = true;



	using KeyCallback = void(*)(fluke::Connection&, uint32_t);
	struct Key {
		unsigned int mod;
		xcb_keysym_t keysym;
		KeyCallback func;
		uint32_t arg;
	};


	void print_p(fluke::Connection& conn, uint32_t arg) {
		tinge::println("key callback `print_p` with arg ", arg);
	}



	constexpr auto MOD     = XCB_MOD_MASK_4;
	constexpr auto CONTROL = XCB_MOD_MASK_CONTROL;
	constexpr auto ALT     = XCB_MOD_MASK_1;
	constexpr auto SHIFT   = XCB_MOD_MASK_SHIFT;

	constexpr std::array keys = {
		Key{ MOD, XK_p, print_p, 1333 },
		// Key{},
		// Key{},
		// Key{},
		// Key{},
		// Key{},
		// Key{},
		// Key{},
		// Key{},
		// Key{},
	};

}

