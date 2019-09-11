#pragma once

#include <vector>
#include <string>

#include <tinge.hpp>
#include <fluke.hpp>

// Move a window in a given direction by a given amount.

namespace fluke::utils {
	int move(fluke::Connection& conn, std::vector<std::string>& args) {
		if (args.size() < 3) {
			tinge::errorln("incorrect number of arguments!");
			return 1;
		}

		// Get the offset to move by.
		decltype(xcb_get_geometry_reply_t::x) x, y;
		x = std::stoi(args[0]);
		y = std::stoi(args[1]);

		auto num_windows = args.size() - 2;


		// Convert window ID strings to integers.
		std::vector<xcb_window_t> windows;
		windows.resize(num_windows);

		for (std::vector<xcb_window_t>::size_type i = 0; i < num_windows; ++i)
			windows[i] = std::strtoul(args[i + 2].c_str(), nullptr, 16);


		// Fire off our get requests.
		std::vector<fluke::GetWindowRect> getters;
		getters.reserve(num_windows); // number of window ids...

		for (auto win: windows)
			getters.emplace_back(conn, win);


		// Get rects of each window.
		std::vector<fluke::Rect> rects;
		rects.reserve(num_windows); // number of window ids...

		for (auto& win: getters) {
			try {
				rects.emplace_back(win.get());

			} catch (const fluke::GetWindowGeometryError&) {
				tinge::errorln("error occured getting window geometry!");
				return 1;
			}
		}


		// Set the new position of each window.
		std::vector<fluke::SetWindowRect> setters;
		setters.reserve(num_windows); // number of window ids...

		for (std::vector<xcb_window_t>::size_type i = 0; i < windows.size(); ++i) {
			auto rect = rects[i];

			rect.x += x;
			rect.y += y;

			setters.emplace_back(conn, windows[i], rect);

			tinge::warnln("moving window ", windows[i], " by (", x, ", ", y, ").");
		}


		// Fire off all the pending requests.
		for (auto& win: setters)
			win.get();

		return 0;
	}
}
