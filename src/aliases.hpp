#pragma once

extern "C" {
	#include <xcb/xcb.h>
}

namespace fluke::xcb {

	using Connection = xcb_connection_t*;
	using Screen     = xcb_screen_t*;
	using Window     = xcb_window_t*;

}
