#pragma once

#include <vector>
#include <string>
#include <utils/core.hpp>
#include <structures/connection.hpp>

namespace fluke::display {
	inline auto list(const fluke::Connection& conn) {
		std::vector<std::string> vec;

		for (const auto& provider: fluke::get_providers(conn, conn)) {
			for (const auto& output: fluke::get_outputs(conn, provider)) {

				if (fluke::get_output_connection(conn, output) == XCB_RANDR_CONNECTION_CONNECTED) {
					auto output_info = get_output_info(conn, output);
					auto output_crtc = get_output_crtc_info(conn, output_info->crtc);

					vec.emplace_back(get_output_name(conn, output));
				}
			}
		}

		return vec;
	}
}
