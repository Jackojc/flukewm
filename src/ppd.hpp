#pragma once

#include <utils/core.hpp>

namespace fluke {
	auto ppd(xcb_connection_t* conn) {
		auto screen = fluke::get_screen(conn);

		xcb_randr_get_output_primary_reply_t *primary_output = fluke::get_primary_output(conn);

		return fluke::get_output_name(conn, primary_output->output);
	}
}
