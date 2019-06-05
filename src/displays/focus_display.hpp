#pragma once

#include <utils/core.hpp>
#include <structures/connection.hpp>

namespace fluke::display {
	// Get name of primary display
	inline auto primary(const fluke::Connection& conn) {
		auto primary_output = fluke::get_primary_output(conn);
		return fluke::get_output_name(conn, primary_output->output);
	}




	// Get name of display with window on it.
	inline std::string window_on(const fluke::Connection& conn, xcb_window_t win) {
		if (not fluke::exists(conn, win))
			fluke::error("window does not exist.");

		auto geom = fluke::get_window_geometry(conn, win);

		const auto in_bounds = [&geom] (auto x, auto y, auto w, auto h) {
			return (
				geom->x >= x and
				geom->y >= y and
				geom->x <= (x + w) and
				geom->y <= (y + h)
			);
		};


		for (const auto& provider: fluke::get_providers(conn, conn)) {
			for (const auto& output: fluke::get_outputs(conn, provider)) {

				if (fluke::get_output_connection(conn, output) == XCB_RANDR_CONNECTION_CONNECTED) {
					auto output_info = get_output_info(conn, output);
					auto output_crtc = get_output_crtc_info(conn, output_info->crtc);

					auto w = output_crtc->width;
					auto h = output_crtc->height;
					auto x = output_crtc->x;
					auto y = output_crtc->y;

					if (in_bounds(x, y, w, h))
						return fluke::get_output_name(conn, output);
				}
			}
		}

		return {};
	}



	// Return name of display with pointer on it.
	inline std::string hovered(const fluke::Connection& conn) {
		auto cookie = xcb_query_pointer(conn, conn.screen->root);
		auto reply  = xcb_query_pointer_reply(conn, cookie, nullptr);


		const auto in_bounds = [&reply] (auto x, auto y, auto w, auto h) {
			return (
				reply->root_x >= x and
				reply->root_y >= y and
				reply->root_x <= (x + w) and
				reply->root_y <= (y + h)
			);
		};


		for (const auto& provider: fluke::get_providers(conn, conn)) {
			for (const auto& output: fluke::get_outputs(conn, provider)) {

				if (fluke::get_output_connection(conn, output) == XCB_RANDR_CONNECTION_CONNECTED) {
					auto output_info = get_output_info(conn, output);
					auto output_crtc = get_output_crtc_info(conn, output_info->crtc);

					auto w = output_crtc->width;
					auto h = output_crtc->height;
					auto x = output_crtc->x;
					auto y = output_crtc->y;

					if (in_bounds(x, y, w, h))
						return fluke::get_output_name(conn, output);
				}
			}
		}

		return {};
	}

}
