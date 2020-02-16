#pragma once

#include <vector>
#include <algorithm>
#include <cmath>
#include <fluke.hpp>


namespace fluke {
	/*
		Sends a series of requests all at once then immediatetely after
		blocks until the replies are fetched.

		A new request is fired off for each argument in `changing_arg`.
		For every request, this is the only argument that changes, the other
		argument remain constant for all requests.

		example:
			auto geoms = fluke::dispatch_consume(conn, [&conn] (xcb_window_t win) {
				return fluke::get_geometry( conn, win );
			}, fluke::get_mapped_windows(conn));

			for (const auto& geom: geoms)
				std::cout << fluke::as_rect(geom) << '\n';
	*/
	template <typename T, typename F, typename... Ts>
	inline decltype(auto) dispatch_consume(
		fluke::Connection& conn, F func, const std::vector<T>& changing_arg, Ts&&... args
	) {
		std::vector<decltype(func(changing_arg.front(), std::forward<Ts>(args)...))> request;
		std::vector<decltype(fluke::get(conn, request.front()))> reply;

		// Fire off a request for each argument in `changing_arg` while passing
		// the unchanging args too.
		for (const auto& x: changing_arg)
			request.emplace_back(func(x, std::forward<Ts>(args)...));

		// Block on each request to fetch it's reply.
		for (const auto& x: request)
			reply.emplace_back(fluke::get(conn, x));

		return reply;
	}



	/*
		Returns a vector of all windows.

		example:
			for (xcb_window_t win: fluke::get_tree(conn)) {
				auto attr = fluke::get(conn, fluke::get_window_attributes(conn, win));
				std::cout << fluke::is_mapped(attr) << '\n';
			}
	*/
	inline auto get_tree(fluke::Connection& conn) {
		// Ask X for a list of windows, returns a pointer to
		// an `xcb_query_tree_reply_t` structure.
		const auto tree = fluke::get(conn, fluke::query_tree(conn, conn.root()));

		// Create a vector using start pointer and end pointer.
		// Each element is copied into the vector.
		auto vec =  std::vector<xcb_window_t>{
			xcb_query_tree_children(tree.get()),  // pointer to array of windows.
			xcb_query_tree_children(tree.get()) + xcb_query_tree_children_length(tree.get())
		};

		std::reverse(vec.begin(), vec.end());

		return vec;
	}



	/*
		This function returns the currently focused window ID.

		example:
			xcb_window_t focused = fluke::get_focused_window(conn);
	*/
	inline auto get_focused_window(fluke::Connection& conn) {
		return fluke::get(conn, fluke::get_input_focus(conn))->focus;
	}



	/*
		Returns a vector of xcb_window_t IDs which contains all of the known windows.
		This includes mapped(visible) and unmapped(invisible) windows.

		example:
			auto windows = fluke::get_all_windows(conn);

			for (xcb_window_t win: windows)
				std::cout << fluke::to_hex(win) << '\n';
	*/
	inline auto get_all_windows(fluke::Connection& conn) {
		// Get all windows.
		auto windows = fluke::get_tree(conn);

		// Get window attributes for every ID in `windows`.
		const auto attrs = fluke::dispatch_consume(conn, [&conn] (xcb_window_t win) {
			return fluke::get_window_attributes( conn, win );
		}, windows);


		// We use `i` to allow indexing `attrs` while simultaneously indexing `windows`.
		decltype(attrs)::size_type i = 0;

		// Remove windows which have override_redirect set, they have asked to
		// not be managed by the window manager.
		const auto should_remove = [&] (xcb_window_t) {
			return fluke::is_ignored(attrs[i++]);
		};

		windows.erase(std::remove_if(windows.begin(), windows.end(), should_remove), windows.end());

		return windows;
	}



	/*
		This is the same as `get_all_windows` except that it filters
		out unmapped(invisible) windows.

		example:
			auto windows = fluke::get_mapped_windows(conn);

			for (xcb_window_t win: windows)
				std::cout << fluke::to_hex(win) << '\n';
	*/
	inline auto get_mapped_windows(fluke::Connection& conn) {
		// Get all windows.
		auto windows = fluke::get_tree(conn);

		// Get window attributes for every ID in `windows`.
		const auto attrs = fluke::dispatch_consume(conn, [&conn] (xcb_window_t win) {
			return fluke::get_window_attributes( conn, win );
		}, windows);


		// We use `i` to allow indexing `attrs` while simultaneously indexing `windows`.
		decltype(attrs)::size_type i = 0;

		// Remove windows which have override_redirect set and are unmapped,
		// they have asked to not be managed by the window manager.
		const auto should_remove = [&] (xcb_window_t) {
			const auto& attr = attrs[i++];

			return
				fluke::is_ignored(attr) or
				not fluke::is_mapped(attr)
			;
		};

		windows.erase(std::remove_if(windows.begin(), windows.end(), should_remove), windows.end());

		return windows;
	}



	/*
		Returns a vector of xcb_randr_provider_t which contains all of the known providers.

		example:
			auto providers = fluke::get_providers(conn);
	*/
	inline auto get_providers(fluke::Connection& conn) {
		const auto provider_info = fluke::get(conn, fluke::randr_get_providers(conn, conn.root()));

		// Create a vector using start pointer and end pointer.
		// Each element is copied into the vector.
		return std::vector<xcb_randr_provider_t>{
			xcb_randr_get_providers_providers(provider_info.get()),  // pointer to array of windows.
			xcb_randr_get_providers_providers(provider_info.get()) +
				xcb_randr_get_providers_providers_length(provider_info.get())
		};
	}



	/*
		Returns a vector of xcb_randr_output_t IDs which contains all of the known displays.

		example:
			auto outputs = fluke::get_provider_info(conn, provider);
	*/
	inline auto get_provider_info(fluke::Connection& conn, xcb_randr_provider_t provider) {
		const auto output_info = fluke::get(conn, fluke::randr_get_provider_info(conn, provider));

		// Create a vector using start pointer and end pointer.
		// Each element is copied into the vector.
		return std::vector<xcb_randr_output_t>{
			xcb_randr_get_provider_info_outputs(output_info.get()),  // pointer to array of outputs.
			xcb_randr_get_provider_info_outputs(output_info.get()) +
				xcb_randr_get_provider_info_outputs_length(output_info.get())
		};
	}



	/*
		Returns a vector of xcb_randr_output_t IDs which contains all of the known displays.
		This functions similarly to the above function
		`get_provider_info` but it's a bit simpler to use.

		example:
			for (auto& output: fluke::get_screen_resources(conn)) {
				auto info = fluke::get(conn, fluke::randr_get_output_info(conn, output));

				if (fluke::is_connected(info)) {
					auto [x, y, w, h] = fluke::as_rect(fluke::get(conn, fluke::randr_get_crtc_info(info->crtc)));
					...
				}
			}
	*/
	inline auto get_screen_resources(fluke::Connection& conn) {
		const auto screen_resources = fluke::get(conn, fluke::randr_get_screen_resources_current(conn, conn.root()));

		// Create a vector using start pointer and end pointer.
		// Each element is copied into the vector.
		return std::vector<xcb_randr_output_t>{
			xcb_randr_get_screen_resources_current_outputs(screen_resources.get()),  // pointer to array of outputs.
			xcb_randr_get_screen_resources_current_outputs(screen_resources.get()) +
				xcb_randr_get_screen_resources_current_outputs_length(screen_resources.get())
		};
	}



	/*
		Returns a vector of xcb_randr_crtc_t structures which contains the
		attributes of all connected displays.

		example:
			for (auto& disp: fluke::get_crtcs(conn)) {
				auto [x, y, w, h] = fluke::as_rect(disp);
				...
			}
	*/
	inline auto get_crtcs(fluke::Connection& conn) {
		const auto outputs = fluke::get_screen_resources(conn);

		// Get output info for each display.
		auto output_info = fluke::dispatch_consume(conn, [&conn] (xcb_randr_output_t out) {
			return fluke::randr_get_output_info(conn, out);
		}, outputs);


		// Remove disconnected displays.
		output_info.erase(std::remove_if(output_info.begin(), output_info.end(), [] (const auto& info) {
			return not fluke::is_connected(info) or info->crtc == XCB_NONE;
		}), output_info.end());


		// Get CRTC structures.
		return fluke::dispatch_consume(conn, [&conn] (const auto& info) {
			return fluke::randr_get_crtc_info(conn, info->crtc);
		}, output_info);
	}



	/*
		Distance algorithm, return the distance between 2 cartesian points on a 2d plane.

		This variant of the function does not return the actual distance, rather, it
		is used for cases where you need to compare two distances and find which one is
		shorter/longer.

		example:
			auto dist = fluke::distance_fast({0, 0}, {5, 5});
	*/
	inline auto distance_fast(const fluke::Point a, const fluke::Point& b) {
		return std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2);
	}



	/*
		Distance algorithm, return the distance between 2 cartesian points on a 2d plane.

		example:
			auto dist = fluke::distance({0, 0}, {5, 5});
	*/
	inline auto distance(const fluke::Point a, const fluke::Point& b) {
		return std::sqrt(std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2));
	}



	/*
		Distance algorithm, return the distance between 2 cartesian points on a 2d plane.

		This variant of the distance algorithm is the Manhatten distance or Taxi Cab distance.

		example:
			auto dist = fluke::distance_abs({0, 0}, {5, 5});
	*/
	inline auto distance_abs(const fluke::Point a, const fluke::Point& b) {
		return std::abs(a.x - b.x) + std::abs(a.y - b.y);
	}



	/*
		Find the rect of a display which is nearest to provided rect.
		Can be used to find the display that a window is on for example.

		example:
			auto [x, y, w, h] = get_nearest_display_rect(conn,
				fluke::as_rect(fluke::get(conn,
					fluke::get_geometry(conn, fluke::get_focused_window(conn))
				))
			);
	*/
	inline fluke::Rect get_nearest_display_rect(fluke::Connection& conn, const fluke::Rect& r) {
		// Destructure rect argument.
		const auto [x_, y_, w_, h_] = r;

		// Get center of rect argument.
		const fluke::Point center{ x_ + w_ / 2, y_ + h_ / 2 };

		// Store distances to rects and the associated rect itself.
		std::vector<std::pair<fluke::Rect, int>> distances;

		// Loop over all displays and get its distance to `center`.
		for (const auto& disp: fluke::get_crtcs(conn)) {
			const auto [x, y, w, h] = fluke::as_rect(disp);

			distances.emplace_back(
				fluke::Rect{x, y, w, h},
				fluke::distance(center, fluke::Point{ x + w / 2, y + h / 2 })
			);
		}

		// Find rect which is nearest and return it.
		return std::min_element(distances.begin(), distances.end(), [] (const auto& a, const auto& b) {
			return a.second < b.second;
		})->first;
	}



	/*
		Gets a vector of keycodes from a supplied keysym.

		example:
			for (auto& keycode: fluke::get_keycodes(conn, key_keysym)) { ... }
	*/
	inline auto get_keycodes(fluke::Connection& conn, xcb_keysym_t sym) {
		// Get a pointer to an array of keycodes.
		const auto keycode_ptr = std::unique_ptr<xcb_keycode_t[], decltype(&std::free)>{
			xcb_key_symbols_get_keycode(conn.keysyms(), sym), &std::free
		};

		// The array we got above is terminated with `XCB_NO_SYMBOL`.
		// Find the length of the array.
		size_t count = 0;
		while (keycode_ptr[count] != XCB_NO_SYMBOL)
			count++;

		// Use pointer + length to create a vector.
		return std::vector<xcb_keycode_t>{
			keycode_ptr.get(),
			keycode_ptr.get() + count
		};
	}



	/*
		Convert a keycode to a keysym.

		example:
			xcb_keysym_t keysym = fluke::get_keysym(conn, e->detail);

	*/
	inline auto get_keysym(fluke::Connection& conn, xcb_keycode_t keycode) {
		return xcb_key_symbols_get_keysym(conn.keysyms(), keycode, 0);
	}



	/*
		Grab all of the keys we want to receive events for. The keys we care about
		are defined in the `keys` array we pass in.

		We grab keys in a way that lets them work regardless of currently
		active modifiers such as caps lock, scroll lock and num lock.

		example:
			register_keybindings(conn, keys);
	*/
	template <size_t N>
	inline void register_keybindings(fluke::Connection& conn, const fluke::Keys<N>& keys) {
		// Toggleable modifiers.
		constexpr std::array modifiers{
			0u,

			fluke::keys::caps_lock,
			fluke::keys::num_lock,
			fluke::keys::scroll_lock,

			fluke::keys::caps_lock | fluke::keys::num_lock,
			fluke::keys::caps_lock | fluke::keys::scroll_lock,
			fluke::keys::num_lock  | fluke::keys::scroll_lock,

			fluke::keys::caps_lock | fluke::keys::num_lock | fluke::keys::scroll_lock,
		};

		// Ungrab any keys which are already grabbed.
		fluke::ungrab_key(conn, XCB_GRAB_ANY, conn.root(), XCB_MOD_MASK_ANY);

		// Register our keybindings.
		FLUKE_DEBUG_NOTICE_SUB("grab keys.")
		for (const auto& [key_mod, key_keysym, key_func]: keys) {
			for (const auto& keycode: fluke::get_keycodes(conn, key_keysym)) {
				// Register the keybind under every modifier in the above structure.
				// This is so that our keybinding can work while various "locks" are
				// active like caps lock.
				// This will make X send us events for the registered bindings.
				for (const auto& mod: modifiers)
					fluke::grab_key(
						conn, true, conn.root(), key_mod | mod, keycode, XCB_GRAB_MODE_ASYNC, XCB_GRAB_MODE_ASYNC
					);
			}
		}
	}



	/*
		Block until next event, wrap it in fluke::Event and return.

		example:
			auto event = fluke::get_next_event(conn);
	*/
	inline auto get_next_event(fluke::Connection& conn) {
		return fluke::Event{xcb_wait_for_event(conn), &std::free};
	}



	/*
		Get the event type of a generic event structure.

		example:
			auto event = fluke::get_next_event(conn);
			auto ev_type = fluke::get_event_type(event);
	*/
	inline auto get_event_type(const fluke::Event& e) {
		return XCB_EVENT_RESPONSE_TYPE(e.get());
	}



	/*
		Get the display area when taking into account the gutters
		around the display edge.

		example:
			auto [x, y, w, h] = fluke::get_adjusted_display_rect(
				fluke::get_nearest_display_rect(conn, focused_rect)
			);
	*/
	inline auto get_adjusted_display_rect(const fluke::Rect& r) {
		auto [x, y, w, h] = r;

		x += fluke::config::GUTTER_LEFT;
		y += fluke::config::GUTTER_TOP;
		w -= fluke::config::GUTTER_RIGHT + fluke::config::GUTTER_LEFT;
		h -= fluke::config::GUTTER_BOTTOM + fluke::config::GUTTER_TOP;

		return fluke::Rect{ x, y, w, h };
	}



	/*
		Get the window size when taking into account the border
		size and window gaps.

		example:
			const auto [x, y, w, h] = fluke::get_adjusted_window_rect({ ... });
	*/
	inline auto get_adjusted_window_rect(const fluke::Rect& r) {
		auto [x, y, w, h] = r;

		x += fluke::config::GAP;
		y += fluke::config::GAP;
		w -= fluke::config::BORDER_SIZE * 2 + fluke::config::GAP * 2;
		h -= fluke::config::BORDER_SIZE * 2 + fluke::config::GAP * 2;

		return fluke::Rect{ x, y, w, h };
	};



	/*
		Gets the center point of a given rectangle.

		example:
			auto [x, y] = fluke::get_rect_center({0, 0, 10, 10});
	*/
	inline auto get_rect_center(const fluke::Rect& r) {
		const auto [x, y, w, h] = r;
		return fluke::Point{ x + w / 2, y + h / 2 };
	}



	/*
		Centers the cursor in the center of the given rect.

		example:
			auto geom = fluke::get(conn, fluke::get_geometry(conn, win));
			fluke::center_pointer_in_rect(conn, fluke::as_rect(geom));
	*/
	inline void center_pointer_in_rect(fluke::Connection& conn, const fluke::Rect& r) {
		auto [x, y] = fluke::get_rect_center(r);
		fluke::warp_pointer(conn, XCB_NONE, conn.root(), 0, 0, 0, 0, x, y);
	}



	/*
		Checks if a given point resides within the boundaries of a given
		rectangle.

		example:
			bool is_inside = fluke::aabb({0, 0, 10, 10}, {5, 5});
	*/
	inline bool aabb(const fluke::Rect& r, const fluke::Point& p) {
		const auto [x, y, w, h] = r;
		const auto [px, py] = p;

		return
			px >= x and
			py >= y and
			px <= x + w and
			py <= y + h
		;
	}



	/*
		Get the rectangle of the monitor which contains the pointer.

		example:
			auto [x, y, w, h] = fluke::get_hovered_monitor(conn);
	*/
	inline fluke::Rect get_hovered_display_rect(fluke::Connection& conn) {
		const auto cursor = fluke::as_point(fluke::get(conn, fluke::query_pointer(conn, conn.root())));

		for (const auto& disp: fluke::get_crtcs(conn)) {
			const auto display_rect = fluke::as_rect(disp);

			if (fluke::aabb(display_rect, cursor))
				return display_rect;
		}

		return fluke::Rect{0, 0, 0, 0};
	}



	/*
		Check if a given window is valid.

		example:
			bool is_valid = fluke::is_valid_window(conn, win);
	*/
	inline bool is_valid_window(fluke::Connection& conn, xcb_window_t win) {
		return win != conn.root() and win != XCB_NONE;
	}



	/*
		Returns a vector of windows which are mapped, not ignored and are on
		the same display as the mouse cursor.

		example:
			auto windows = fluke::get_mapped_windows_on_hovered_display(conn);

			for (xcb_window_t win: windows)
				std::cout << fluke::to_hex(win) << '\n';
	*/
	inline auto get_mapped_windows_on_hovered_display(fluke::Connection& conn) {
		// Get all windows.
		auto windows = fluke::get_tree(conn);

		// Get the geometry and attributes of all windows,
		// each element in the returned vector is of type
		// `std::tuple<fluke::GetWindowAttributesReply, fluke::GetGeometryReply>`.
		const auto attrs_geoms = fluke::dispatch_consume(conn, [&conn] (xcb_window_t win) {
			return std::tuple{
				fluke::get_window_attributes(conn, win),
				fluke::get_geometry(conn, win)
			};
		}, windows);


		// Get the rect of the display which contains the mouse cursor.
		auto hovered_rect = fluke::get_hovered_display_rect(conn);

		// We use `i` to allow indexing `attrs_geoms` while simultaneously indexing `windows`.
		decltype(attrs_geoms)::size_type i = 0;

		// Remove windows which have override_redirect set, are unmapped and
		// which are not on the same diplay as the pointer.
		const auto should_remove = [&] (xcb_window_t win) {
			const auto& [attr, geom] = attrs_geoms[i++];

			return
				fluke::is_ignored(attr) or
				not fluke::is_mapped(attr) or
				hovered_rect != fluke::get_nearest_display_rect(conn, fluke::as_rect(geom))
			;
		};

		windows.erase(std::remove_if(windows.begin(), windows.end(), should_remove), windows.end());

		return windows;
	}



	/*
		This function will center and resize a window on the currently hovered display.

		example:
			xcb_window_t focused = fluke::get_focused_window(conn);
			fluke::center_resize_window_on_hovered_display(conn, focused);
	*/
	inline void center_resize_window_on_hovered_display(fluke::Connection& conn, xcb_window_t win) {
		// Get rect of focused display.
		const auto [display_x, display_y, display_w, display_h] =
			fluke::get_hovered_display_rect(conn);

		// Resize window to a percentage of the screen size.
		const auto w = (display_w * fluke::config::NEW_WINDOW_PERCENT) / 100;
		const auto h = (display_h * fluke::config::NEW_WINDOW_PERCENT) / 100;

		// Center the window on the screen.
		const auto x = (display_x + display_w / 2) - w / 2;
		const auto y = (display_y + display_h / 2) - h / 2;

		// Register to receive events from the window and resize/move the window.
		fluke::configure_window(conn, win, fluke::XCB_MOVE_RESIZE, x, y, w, h);
	}



	/*
		This function will center a window on the currently hovered display.

		example:
			xcb_window_t focused = fluke::get_focused_window(conn);
			fluke::center_window_on_hovered_display(conn, focused);
	*/
	inline void center_window_on_hovered_display(fluke::Connection& conn, xcb_window_t win) {
		// Get window geometry.
		const auto [window_x, window_y, window_w, window_h] =
			fluke::as_rect(fluke::get(conn, fluke::get_geometry(conn, win)));

		// Get rect of focused display.
		const auto [display_x, display_y, display_w, display_h] =
			fluke::get_hovered_display_rect(conn);

		// Center the window on the screen.
		const auto x = (display_x + display_w / 2) - window_w / 2;
		const auto y = (display_y + display_h / 2) - window_h / 2;

		fluke::configure_window(conn, win, fluke::XCB_MOVE, x, y);
	}



	/*
		Get the current cursor position as a 2d coordinate.
	*/
	inline auto get_pointer_point(fluke::Connection& conn) {
		return fluke::as_point(fluke::get(conn, fluke::query_pointer(conn, conn.root())));
	}



	/**/
	inline auto remove_focused_window(fluke::Connection& conn, std::vector<xcb_window_t>& windows) {
		auto focused = fluke::get_focused_window(conn);
		windows.erase(std::remove(windows.begin(), windows.end(), focused), windows.end());
	}
}

