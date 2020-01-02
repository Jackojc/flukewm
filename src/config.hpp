#pragma once

#include <cstdint>

namespace fluke::config {
	// argb format
	constexpr uint32_t BORDER_COLOUR_INACTIVE = 0xff000000;
	constexpr uint32_t BORDER_COLOUR_ACTIVE   = 0xffffffff;

	constexpr uint32_t BORDER_SIZE = 2;

	// if cursor moves onto root window, should focus be lost on current window?
	constexpr bool USE_LAZY_FOCUS = true;

}

