#pragma once

#include <vector>
#include <algorithm>
#include <sstream>
#include <string>
#include <cmath>
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


	#define FLUKE_DEBUG_NOTICE(...) FLUKE_DEBUG( tinge::noticeln(__VA_ARGS__) )
	#define FLUKE_DEBUG_WARN(...) FLUKE_DEBUG( tinge::warnln(__VA_ARGS__) )
	#define FLUKE_DEBUG_ERROR(...) FLUKE_DEBUG( tinge::errorln(__VA_ARGS__) )
	#define FLUKE_DEBUG_SUCCESS(...) FLUKE_DEBUG( tinge::successln(__VA_ARGS__) )



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
			dispatch_consume(conn, [] (auto win, auto&&... args) {
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
		const auto should_remove = [&attrs, &i] (xcb_window_t) {
			return fluke::is_ignored(attrs[i++]);
		};

		windows.erase(std::remove_if(windows.begin(), windows.end(), should_remove), windows.end());

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
		const auto should_remove = [&attrs, &i] (xcb_window_t win) {
			const auto& attr = attrs[i++];
			return fluke::is_ignored(attr) or not fluke::is_mapped(attr);
		};

		windows.erase(std::remove_if(windows.begin(), windows.end(), should_remove), windows.end());

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
			return not fluke::is_connected(info) or info->crtc == XCB_NONE;
		}), output_info.end());


		// Get CRTC structures.
		auto crtcs = fluke::dispatch_consume(conn, [&conn] (const auto& info) {
			return fluke::randr_get_crtc_info( conn, info->crtc );
		}, output_info);

		return crtcs;
	}



	/*
		Find the rect of a display which is nearest to provided rect.
		Can be used to find the display that a window is on for example.

		example:
			get_nearest_display_rect(conn, rect);
	*/
	inline fluke::Rect get_nearest_display_rect(fluke::Connection& conn, const fluke::Rect& r) {
		// Destructure rect argument.
		auto [x_, y_, w_, h_] = r;

		// Get center of rect argument.
		fluke::Point center{ x_ + w_ / 2, y_ + h_ / 2 };

		// Store distances to rects and the associated rect itself.
		std::vector<std::pair<fluke::Rect, int>> distances;

		// Distance algorithm.
		// Compare center of display to center of rect argument.
		const auto distance = [&center] (const fluke::Point& p) {
			return std::sqrt(std::pow(center.x - p.x, 2) + std::pow(center.y - p.y, 2));
		};

		// Loop over all displays and get its distance to `center`.
		for (auto& disp: fluke::get_crtcs(conn)) {
			auto [x, y, w, h] = fluke::as_rect(disp);
			distances.emplace_back(fluke::Rect{x, y, w, h}, distance(fluke::Point{ x + w / 2, y + h / 2 }));
		}

		// Find rect which is nearest and return it.
		return std::min_element(distances.begin(), distances.end(), [] (const auto& a, const auto& b) {
			return a.second < b.second;
		})->first;
	}



	/*
		Gets a vector of keycodes from a supplied keysym.

		example:
			get_keycodes(conn, keysym);
	*/
	inline auto get_keycodes(fluke::Connection& conn, xcb_keysym_t sym) {
		// Get a pointer to an array of keycodes.
		auto keycode_ptr = std::unique_ptr<xcb_keycode_t[], decltype(&std::free)>{
			xcb_key_symbols_get_keycode(conn.keysyms(), sym), &std::free
		};

		// The array we got above is terminated with `XCB_NO_SYMBOL`.
		// Find the length of the array.
		size_t count = 0;
		while (keycode_ptr[count] != XCB_NO_SYMBOL)
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

		// Ungrab any keys which are already grabbed.
		fluke::ungrab_key(conn, XCB_GRAB_ANY, conn.root(), fluke::XCB_MASK_ANY);

		// Register our keybindings.
		for (auto& [key_mod, key_keysym, key_func]: keys) {
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

