#pragma once

#include <utils/core.hpp>
#include <structures/rectangle.hpp>
#include <structures/connection.hpp>

namespace fluke::display {
	inline Rectangle attr(const fluke::Connection& conn, const std::string& display) {
		for (const auto& provider: fluke::get_providers(conn, conn)) {
			for (const auto& output: fluke::get_outputs(conn, provider)) {
				if (
					fluke::get_output_connection(conn, output) == XCB_RANDR_CONNECTION_CONNECTED and
					fluke::get_output_name(conn, output) == display
				) {
					auto output_info = fluke::get_output_info(conn, output);
					auto output_crtc = fluke::get_output_crtc_info(conn, output_info->crtc);

					auto x = static_cast<int>(output_crtc->x);
					auto y = static_cast<int>(output_crtc->y);
					auto w = static_cast<int>(output_crtc->width);
					auto h = static_cast<int>(output_crtc->height);

					return { x, y, w, h };
				}
			}
		}

		fluke::error("could not find display.");
		return { 0, 0, 0, 0 };
	}
}
