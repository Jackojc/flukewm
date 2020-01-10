#pragma once

#include <vector>
#include <algorithm>
#include <sstream>
#include <string>
#include <fluke.hpp>


namespace fluke {
	/*
		Macro for toggling code on/off depending on whether we are in release or
		debug builds.

		example:
			FLUKE_DEBUG( std::cerr << "debug string\n" )
	*/
	#ifdef NDEBUG
		#define FLUKE_DEBUG(x) { }
	#else
		#define FLUKE_DEBUG(x) { x; }
	#endif



	/*
		Converts a numeric argument to hexadecimal format with 0x prepended.

		example:
			to_hex(123);
	*/
	template <typename T>
	inline std::string to_hex(T&& arg) {
		std::stringstream ss;
		ss << "0x" << std::hex << arg;
		return ss.str();
	}



	/*
		Sends a series of requests all at once then immediatetely after
		blocks until the replies are fetched.

		A new request is fired off for each argument in `changing_arg`.
		For every request, this is the only argument that changes, the other
		argument remain constant for all requests.

		example:
			dispatch_consume(conn, [] (auto&& win, auto&&... args) {
				return fluke::get_window_attributes( args..., win );
			}, windows);
	*/
	template <typename T, typename F, typename... Ts>
	inline auto dispatch_consume(fluke::Connection& conn, F func, const std::vector<T>& changing_arg, Ts&&... args) {
		std::vector<decltype(func(changing_arg.at(0), std::forward<Ts>(args)...))> request;
		std::vector<decltype(fluke::get(conn, request.front()))> reply;

		// Fire off a request for each argument in `changing_arg` while passing
		// the unchanging args too.
		for (auto& x: changing_arg)
			request.emplace_back(func(x, std::forward<Ts>(args)...));

		// Block on each request to fetch it's reply.
		for (const auto& x: request)
			reply.emplace_back(fluke::get(conn, x));

		return reply;
	}



	/*
		Returns a vector of all windows.

		example:
			get_tree(conn);
	*/
	inline auto get_tree(fluke::Connection& conn) {
		// Ask X for a list of windows, returns a pointer to
		// an `xcb_query_tree_reply_t` structure.
		auto tree = fluke::get(conn, fluke::query_tree(conn, conn.root()));

		// Create a vector using start pointer and end pointer.
		// Each element is copied into the vector.
		std::vector<xcb_window_t> windows{
			xcb_query_tree_children(tree.get()),  // pointer to array of windows.
			xcb_query_tree_children(tree.get()) + xcb_query_tree_children_length(tree.get())
		};

		return windows;
	}



	/*
		Returns a vector of xcb_window_t IDs which contains all of the known windows.

		example:
			get_all_windows(conn);
	*/
	inline auto get_all_windows(fluke::Connection& conn) {
		// Get all windows.
		auto windows = fluke::get_tree(conn);

		// Get window attributes for every ID in `windows`.
		auto attrs = fluke::dispatch_consume(conn, [&conn] (xcb_window_t win) {
			return fluke::get_window_attributes( conn, win );
		}, windows);


		// We use `i` to allow indexing `attrs` while simultaneously indexing `windows`.
		decltype(attrs)::size_type i = 0;

		// Remove windows which have override_redirect set, they have asked to
		// not be managed by the window manager.
		const auto pred = [&attrs, &i] (xcb_window_t) {
			return attrs[i++]->override_redirect;
		};

		windows.erase(std::remove_if(windows.begin(), windows.end(), pred), windows.end());

		return windows;
	}



	/*
		This is the same as `get_all_windows` except that it filters
		out unmapped(invisible) windows.

		example:
			get_mapped_windows(conn);
	*/
	inline auto get_mapped_windows(fluke::Connection& conn) {
		// Get all windows.
		auto windows = fluke::get_tree(conn);

		// Get window attributes for every ID in `windows`.
		auto attrs = fluke::dispatch_consume(conn, [&conn] (xcb_window_t win) {
			return fluke::get_window_attributes( conn, win );
		}, windows);


		// We use `i` to allow indexing `attrs` while simultaneously indexing `windows`.
		decltype(attrs)::size_type i = 0;

		// Remove windows which have override_redirect set and are unmapped,
		// they have asked to not be managed by the window manager.
		const auto pred = [&attrs, &i] (xcb_window_t) {
			return attrs[i]->override_redirect and not(attrs[i++]->map_state == XCB_MAP_STATE_VIEWABLE);
		};

		windows.erase(std::remove_if(windows.begin(), windows.end(), pred), windows.end());

		return windows;
	}



	/*
		Returns a vector of xcb_randr_provider_t which contains all of the known providers.

		example:
			get_providers(conn);
	*/
	inline auto get_providers(fluke::Connection& conn) {
		auto provider_info = fluke::get(conn, fluke::randr_get_providers(conn, conn.root()));

		// Create a vector using start pointer and end pointer.
		// Each element is copied into the vector.
		std::vector<xcb_randr_provider_t> providers{
			xcb_randr_get_providers_providers(provider_info.get()),  // pointer to array of windows.
			xcb_randr_get_providers_providers(provider_info.get()) + xcb_randr_get_providers_providers_length(provider_info.get())
		};

		return providers;
	}



	/*
		Returns a vector of xcb_randr_output_t IDs which contains all of the known displays.

		example:
			get_provider_info(conn, provider);
	*/
	inline auto get_provider_info(fluke::Connection& conn, xcb_randr_provider_t provider) {
		auto output_info = fluke::get(conn, fluke::randr_get_provider_info(conn, provider));

		// Create a vector using start pointer and end pointer.
		// Each element is copied into the vector.
		std::vector<xcb_randr_output_t> outputs{
			xcb_randr_get_provider_info_outputs(output_info.get()),  // pointer to array of outputs.
			xcb_randr_get_provider_info_outputs(output_info.get()) + xcb_randr_get_provider_info_outputs_length(output_info.get())
		};

		return outputs;
	}



	/*
		Returns a vector of screen resources.

		example:
			get_screen_resources(conn);
	*/
	inline auto get_screen_resources(fluke::Connection& conn) {
		auto screen_resources = fluke::get(conn, fluke::randr_get_screen_resources_current(conn, conn.root()));

		// Create a vector using start pointer and end pointer.
		// Each element is copied into the vector.
		std::vector<xcb_randr_output_t> outputs{
			xcb_randr_get_screen_resources_current_outputs(screen_resources.get()),  // pointer to array of outputs.
			xcb_randr_get_screen_resources_current_outputs(screen_resources.get()) +
				xcb_randr_get_screen_resources_current_outputs_length(screen_resources.get())
		};

		return outputs;
	}



	/*
		Returns a vector of xcb_randr_crtc_t structures which contains the
		attributes of all connected displays.

		example:
			get_output_attributes(conn);
	*/
	inline auto get_crtcs(fluke::Connection& conn) {
		auto outputs = fluke::get_screen_resources(conn);

		// Get output info for each display.
		auto output_info = fluke::dispatch_consume(conn, [&conn] (xcb_randr_output_t out) {
			return fluke::randr_get_output_info( conn, out );
		}, outputs);


		// Remove disconnected displays.
		output_info.erase(std::remove_if(output_info.begin(), output_info.end(), [] (const auto& info) {
			return info->connection == XCB_RANDR_CONNECTION_DISCONNECTED or info->crtc == XCB_NONE;
		}), output_info.end());


		// Get CRTC structures.
		auto crtcs = fluke::dispatch_consume(conn, [&conn] (const auto& info) {
			return fluke::randr_get_crtc_info( conn, info->crtc );
		}, output_info);

		return crtcs;
	}



	/*
		Get the display which the window geometry is on.

		example:
			get_focused_display(conn);
	*/
	// template <typename T>
	// inline auto get_focused_display_for_geometry(fluke::Connection& conn, const T& geom) {
	// 	auto crtcs = fluke::get_crtcs(conn);
	// 	auto&& disp_ret = crtcs.front();

	// 	auto win_x = geom->x;
	// 	auto win_y = geom->y;
	// 	auto win_width = geom->width;
	// 	auto win_height = geom->height;

	// 	auto center_x = win_x + (win_width / 2);
	// 	auto center_y = win_y + (win_height / 2);

	// 	for (auto&& disp: crtcs) {
	// 		auto disp_x = disp->x;
	// 		auto disp_y = disp->y;
	// 		auto disp_width = disp->width;
	// 		auto disp_height = disp->height;

	// 		bool on_display =
	// 			center_x >= disp_x and
	// 			center_y >= disp_y and
	// 			center_x <= (disp_x + disp_width) and
	// 			center_y <= (disp_y + disp_height);

	// 		return std::move(disp);
	// 	}

	// 	return std::move(disp_ret);
	// }



	/*
		Gets a vector of keycodes from a supplied keysym.

		example:
			get_keycodes(conn, keysym);
	*/
	auto get_keycodes(fluke::Connection& conn, xcb_keysym_t sym) {
		// Get a pointer to an array of keycodes.
		auto keycode_ptr = std::unique_ptr<xcb_keycode_t, decltype(&std::free)>{
			xcb_key_symbols_get_keycode(conn.keysyms(), sym), &std::free
		};

		// The array we got above is terminated with `XCB_NO_SYMBOL`.
		// Find the length of the array.
		int count = 0;
		while (keycode_ptr.get()[count] != XCB_NO_SYMBOL)
			count++;

		// Use pointer + length to create a vector.
		std::vector<xcb_keycode_t> keycodes{
			keycode_ptr.get(),
			keycode_ptr.get() + count
		};

		return keycodes;
	}



	/*
		Convert a keycode to a keysym.

		example:
			get_keysym(conn, keycode);
	*/
	auto get_keysym(fluke::Connection& conn, xcb_keycode_t keycode) {
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
			uint32_t{0},
			uint32_t{fluke::XCB_MASK_CAPS_LOCK},
			uint32_t{fluke::XCB_MASK_NUM_LOCK},
			uint32_t{fluke::XCB_MASK_SCROLL_LOCK},
		};

		// Get every combination of modifiers.
		std::vector<uint32_t> modifiers_combinations;

		// This is a bit untidy but it produces every combination of modifiers.
		for (auto a: modifiers) {
			for (auto b: modifiers) {
				for (auto c: modifiers) {
					modifiers_combinations.emplace_back( a | b | c);
				}
			}
		}

		// tinge::println(keys.size());
		// tinge::println(modifiers_combinations.size());

		// Ungrab any keys which are already grabbed.
		fluke::ungrab_key(conn, XCB_GRAB_ANY, conn.root(), fluke::XCB_MASK_ANY);

		// Register our keybindings.
		for (auto& [key_mod, key_keysym, key_func, key_arg]: keys) {
			for (auto& keycode: fluke::get_keycodes(conn, key_keysym)) {
				// Register the keybind under every modifier in the above structure.
				// This is so that our keybinding can work while various "locks" are
				// active like caps lock.
				// This will make X send us events for the registered bindings.
				for (auto& mod: modifiers_combinations)
					fluke::grab_key(conn, true, conn.root(), key_mod | mod, keycode, XCB_GRAB_MODE_ASYNC, XCB_GRAB_MODE_ASYNC);
			}
		}
	}
}

