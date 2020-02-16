#pragma once

#include <type_traits>
#include <memory>
#include <tuple>
#include <utility>
#include <cstdlib>
#include <fluke.hpp>


namespace fluke {
	/*
		This structure is a wrapper around xcb cookie types which are used
		as a kind of "future" to retrieve a reply when needed.
	*/
	template <typename C>
	struct Cookie {
		using cookie_t = C;

		cookie_t cookie;

		// Constructors.
		constexpr Cookie() {}
		constexpr Cookie(cookie_t cookie_): cookie{cookie_} {}
		constexpr Cookie(const Cookie& other): cookie{other.cookie} {}

		// Assignment operator.
		constexpr decltype(auto) operator=(const Cookie& other) noexcept {
			if (this != &other)
				cookie = other.cookie;

			return *this;
		}

		// Implicit cast operator.
		operator cookie_t() const noexcept {
			return cookie;
		}
	};





	/*
		This macro is used to define a cookie structure, unique_ptr alias
		and overloaded get function.

		GET_REQUEST(InternAtom, intern_atom) defines the following:
			struct InternAtomCookie { ... };
			using InternAtomReply = ... ;
			inline auto get( ... ) { ... }
	*/
	#define NEW_REQUEST(name, type) \
		struct name##Cookie: Cookie<xcb_##type##_cookie_t> { \
			template <typename... Ts> constexpr name##Cookie(cookie_t cookie_): \
				Cookie::Cookie{cookie_} {} \
		}; \
		using name##Reply = std::unique_ptr<xcb_##type##_reply_t, decltype(&std::free)>; \
		inline auto get(fluke::Connection& conn, const name##Cookie& cookie) { \
			return name##Reply{xcb_##type##_reply(conn, cookie, nullptr), std::free}; \
		}


	NEW_REQUEST(InternAtom,                     intern_atom)
	NEW_REQUEST(GetWindowAttributes,            get_window_attributes)
	NEW_REQUEST(GetGeometry,                    get_geometry)
	NEW_REQUEST(GetProperty,                    get_property)
	NEW_REQUEST(GetInputFocus,                  get_input_focus)
	NEW_REQUEST(QueryTree,                      query_tree)
	NEW_REQUEST(QueryPointer,                   query_pointer)
	NEW_REQUEST(QueryExtension,                 query_extension)
	NEW_REQUEST(RandrGetProviders,              randr_get_providers)
	NEW_REQUEST(RandrGetProviderInfo,           randr_get_provider_info)
	NEW_REQUEST(RandrGetOutputInfo,             randr_get_output_info)
	NEW_REQUEST(RandrGetCrtcInfo,               randr_get_crtc_info)
	NEW_REQUEST(RandrGetOutputPrimary,          randr_get_output_primary)
	NEW_REQUEST(RandrGetScreenResourcesCurrent, randr_get_screen_resources_current)
	NEW_REQUEST(GrabPointer,                    grab_pointer)


	#undef NEW_REQUEST




	/*
		This function is simply used to make the dispatch-consume pattern
		more convenient for a number of get requests.

		example:
			auto [geom, attr] = fluke::get(conn,
				fluke::get_geometry(conn, win),
				fluke::get_window_attributes(conn, win)
			);
	*/
	template <typename T1, typename T2, typename... Ts>
	inline auto get(fluke::Connection& conn, T1&& arg1, T2&& arg2, Ts&&... args) {
		// This is kind of messy but required to not conflict with the overloaded variants defined above.
		return std::tuple{
			fluke::get(conn, std::forward<T1>(arg1)),
			fluke::get(conn, std::forward<T2>(arg2)),
			fluke::get(conn, std::forward<Ts>(args))...
		};
	}



	/*
		This function will call `fluke::get()` on all elements of a tuple and return
		a new tuple with the returned values.

		example:
			auto attrs_geoms = fluke::dispatch_consume(conn, [&conn] (xcb_window_t win) {
				return std::tuple{
					fluke::get_window_attributes(conn, win),
					fluke::get_geometry(conn, win)
				};
			}, windows);

		// The above example will return a vector of tuples.
		// `std::vector<std::tuple<fluke::GetWindowAttributesReply, fluke::GetGeometryReply>>`
	*/
	template <typename... Ts>
	inline decltype(auto) get(fluke::Connection& conn, std::tuple<Ts...> tup) {
		return std::tuple_cat(std::apply([&conn] (auto&&... args) {
			return std::tuple{fluke::get(conn, std::forward<Ts>(args))...};
		}, tup));
	}






	// Getter functions
	inline InternAtomCookie intern_atom(fluke::Connection& conn, bool only_if_exists, std::string_view name) {
		return xcb_intern_atom_unchecked(conn, only_if_exists, name.size(), name.data());
	}


	inline GetWindowAttributesCookie get_window_attributes(fluke::Connection& conn, xcb_window_t win) {
		return xcb_get_window_attributes_unchecked(conn, win);
	}


	inline GetGeometryCookie get_geometry(fluke::Connection& conn, xcb_drawable_t draw) {
		return xcb_get_geometry_unchecked(conn, draw);
	}


	inline GetPropertyCookie get_property(
		fluke::Connection& conn,
		bool delete_,
		xcb_window_t win,
		xcb_atom_t property,
		xcb_atom_t type,
		uint32_t long_offset,
		uint32_t long_length
	) {
		return xcb_get_property_unchecked(conn, delete_, win, property, type, long_offset, long_length);
	}


	inline GetInputFocusCookie get_input_focus(fluke::Connection& conn) {
		return xcb_get_input_focus_unchecked(conn);
	}


	inline QueryTreeCookie query_tree(fluke::Connection& conn, xcb_window_t win) {
		return xcb_query_tree_unchecked(conn, win);
	}


	inline QueryPointerCookie query_pointer(fluke::Connection& conn, xcb_window_t win) {
		return xcb_query_pointer_unchecked(conn, win);
	}






	inline RandrGetProvidersCookie randr_get_providers(fluke::Connection& conn, xcb_window_t win) {
		return xcb_randr_get_providers_unchecked(conn, win);
	}


	inline RandrGetProviderInfoCookie randr_get_provider_info(fluke::Connection& conn, xcb_randr_provider_t provider) {
		return xcb_randr_get_provider_info_unchecked(conn, provider, XCB_CURRENT_TIME);
	}


	inline RandrGetOutputInfoCookie randr_get_output_info(fluke::Connection& conn, xcb_randr_output_t provider) {
		return xcb_randr_get_output_info_unchecked(conn, provider, XCB_CURRENT_TIME);
	}


	inline RandrGetCrtcInfoCookie randr_get_crtc_info(fluke::Connection& conn, xcb_randr_crtc_t crtc) {
		return xcb_randr_get_crtc_info_unchecked(conn, crtc, XCB_CURRENT_TIME);
	}


	inline RandrGetOutputPrimaryCookie randr_get_output_primary(fluke::Connection& conn, xcb_window_t win) {
		return xcb_randr_get_output_primary_unchecked(conn, win);
	}


	inline RandrGetScreenResourcesCurrentCookie randr_get_screen_resources_current(
		fluke::Connection& conn, xcb_window_t win
	) {
		return xcb_randr_get_screen_resources_current_unchecked(conn, win);
	}


	inline GrabPointerCookie grab_pointer(
		fluke::Connection& conn,
		bool owner_events,
		xcb_window_t grab_window,
		uint16_t event_mask,
		uint8_t pointer_mode,
		uint8_t keyboard_mode,
		xcb_window_t confine_to,
		xcb_cursor_t cursor
	) {
		return xcb_grab_pointer_unchecked(
			conn,
			owner_events,
			grab_window,
			event_mask,
			pointer_mode,
			keyboard_mode,
			confine_to,
			cursor,
			XCB_CURRENT_TIME
		);
	}








	// Setter functions
	template <typename T, typename... Ts>
	inline void configure_window(
		fluke::Connection& conn, xcb_window_t win, uint16_t value_mask, T&& arg, Ts&&... args
	) {
		const uint32_t values[] = {
			static_cast<uint32_t>(std::forward<T>(arg)),
			static_cast<uint32_t>(std::forward<Ts>(args))...
		};
		xcb_configure_window(conn, win, value_mask, values);
	}

	inline void configure_window(
		fluke::Connection& conn, xcb_window_t win, uint16_t value_mask, uint32_t* args
	) {
		xcb_configure_window(conn, win, value_mask, args);
	}





	template <typename T, typename... Ts>
	inline void change_window_attributes(
		fluke::Connection& conn, xcb_window_t win, uint16_t value_mask, T&& arg, Ts&&... args
	) {
		const uint32_t values[] = {
			static_cast<uint32_t>(std::forward<T>(arg)),
			static_cast<uint32_t>(std::forward<Ts>(args))...
		};
		xcb_change_window_attributes(conn, win, value_mask, values);
	}

	inline void change_window_attributes(
		fluke::Connection& conn, xcb_window_t win, uint16_t value_mask, uint32_t* args
	) {
		xcb_change_window_attributes(conn, win, value_mask, args);
	}






	inline void set_input_focus(fluke::Connection& conn, uint8_t revert_to, xcb_window_t focus) {
		xcb_set_input_focus(conn, revert_to, focus, XCB_CURRENT_TIME);
	}


	inline void map_window(fluke::Connection& conn, xcb_window_t win) {
		xcb_map_window(conn, win);
	}


	inline void unmap_window(fluke::Connection& conn, xcb_window_t win) {
		xcb_unmap_window(conn, win);
	}



	inline void warp_pointer(
		fluke::Connection& conn,
		xcb_window_t src, xcb_window_t dest,
		int16_t src_x, int16_t src_y,
		uint16_t src_width, uint16_t src_height,
		int16_t dest_x, int16_t dest_y
	) {
		xcb_warp_pointer(conn, src, dest, src_x, src_y, src_width, src_height, dest_x, dest_y);
	}



	inline void grab_key(
		fluke::Connection& conn,
		bool owner_events,
		xcb_window_t grab_window,
		uint16_t modifiers,
		xcb_keycode_t key,
		uint8_t pointer_mode,
		uint8_t keyboard_mode
	) {
		xcb_grab_key(conn, owner_events, grab_window, modifiers, key, pointer_mode, keyboard_mode);
	}



	inline void ungrab_key(fluke::Connection& conn, xcb_keycode_t key, xcb_window_t grab_window, uint16_t modifiers) {
		xcb_ungrab_key(conn, key, grab_window, modifiers);
	}


	template <typename T>
	inline void send_event(fluke::Connection& conn, bool propagate, xcb_window_t win, uint32_t event_mask, T event) {
		xcb_send_event(conn, propagate, win, event_mask, reinterpret_cast<const char*>(event));
	}


	inline void randr_select_input(fluke::Connection& conn, xcb_window_t win, uint16_t mask) {
		xcb_randr_select_input(conn, win, mask);
	}



	inline void ungrab_pointer(fluke::Connection& conn) {
		xcb_ungrab_pointer(conn, XCB_CURRENT_TIME);
	}













	// Convenience functions
	inline auto as_rect(const GetGeometryReply& geom) {
		return fluke::Rect{
			geom->x,
			geom->y,
			geom->width,
			geom->height
		};
	}

	inline auto as_rect(const RandrGetCrtcInfoReply& crtc) {
		return fluke::Rect{
			crtc->x,
			crtc->y,
			crtc->width,
			crtc->height
		};
	}


	inline auto as_point(const QueryPointerReply& pointer) {
		return fluke::Point{
			pointer->root_x,
			pointer->root_y
		};
	}


	inline bool is_ignored(const GetWindowAttributesReply& attr) {
		return attr->override_redirect;
	}


	inline bool is_mapped(const GetWindowAttributesReply& attr) {
		return attr->map_state == XCB_MAP_STATE_VIEWABLE;
	}


	inline bool is_connected(const RandrGetOutputInfoReply& info) {
		return info->connection == XCB_RANDR_CONNECTION_CONNECTED;
	}


}

