#pragma once

#include <algorithm>
#include <utils/core.hpp>
#include <structures/connection.hpp>

namespace fluke::window::list {
	inline auto all(const fluke::Connection& conn) {
		auto windows = fluke::get_windows(conn, conn.screen->root);

		if (windows.size() == 0)
			fluke::error("could not get windows.");

		return windows;
	}


	inline auto hidden(const fluke::Connection& conn) {
		auto windows = fluke::get_windows(conn, conn.screen->root);

		if (windows.size() == 0)
			fluke::error("could not get windows.");

		// Remove windows not satisfying the mask provided by caller.
		const auto pred = [&conn] (const auto& elem) {
			return not is_mapped(conn, elem);
		};

		windows.erase(std::remove_if(windows.begin(), windows.end(), pred), windows.end());

		return windows;
	}


	inline auto ignored(const fluke::Connection& conn) {
		auto windows = fluke::get_windows(conn, conn.screen->root);

		if (windows.size() == 0)
			fluke::error("could not get windows.");

		// Remove windows not satisfying the mask provided by caller.
		const auto pred = [&conn] (const auto& elem) {
			return not is_ignored(conn, elem);
		};

		windows.erase(std::remove_if(windows.begin(), windows.end(), pred), windows.end());

		return windows;
	}


	inline auto normal(const fluke::Connection& conn) {
		auto windows = fluke::get_windows(conn, conn.screen->root);

		if (windows.size() == 0)
			fluke::error("could not get windows.");

		// Remove windows not satisfying the mask provided by caller.
		const auto pred = [&conn] (const auto& elem) {
			return not(is_mapped(conn, elem) and is_ignored(conn, elem));
		};

		windows.erase(std::remove_if(windows.begin(), windows.end(), pred), windows.end());

		return windows;
	}
}
