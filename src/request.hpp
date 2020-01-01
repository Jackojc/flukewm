#pragma once

#include <type_traits>
#include <memory>
#include <utility>
#include <cstdlib>
#include <fluke.hpp>


namespace fluke {
	template <typename C>
	struct Cookie {
		using cookie_t = C;

		cookie_t cookie;

		constexpr Cookie() {}
		constexpr Cookie(cookie_t cookie_): cookie{cookie_} {}
		constexpr Cookie(const Cookie& other): cookie{other.cookie} {}

		constexpr decltype(auto) operator=(const Cookie& other) {
			if (this != &other)
				cookie = other.cookie;

			return *this;
		}

		operator cookie_t() const {
			return cookie;
		}
	};





	// Define a get request, nothing special.
	#define GET_REQUEST(name, type) \
		struct name##Cookie: Cookie<xcb_##type##_cookie_t> { \
			template <typename... Ts> constexpr name##Cookie(cookie_t cookie_): \
				Cookie::Cookie{cookie_} {} \
		}; \
		auto get(fluke::Connection& conn, const name##Cookie& cookie) { \
			if (auto ret = std::unique_ptr<xcb_##type##_reply_t, decltype(&std::free)>{xcb_##type##_reply(conn, cookie, nullptr), std::free}) \
				return ret; \
			throw fluke::name##Error{}; \
		}


	// Define a set request, uses the same cookie and reply function for all setters.
	#define SET_REQUEST(name, type) \
		struct name##Cookie: Cookie<xcb_void_cookie_t> { \
			template <typename... Ts> constexpr name##Cookie(cookie_t cookie_): \
				Cookie::Cookie{cookie_} {} \
		};



	// Getters
	GET_REQUEST(InternAtom,            intern_atom)
	GET_REQUEST(GetWindowAttributes,   get_window_attributes)
	GET_REQUEST(GetGeometry,           get_geometry)
	GET_REQUEST(GetProperty,           get_property)
	GET_REQUEST(GetInputFocus,         get_input_focus)
	GET_REQUEST(QueryTree,             query_tree)
	GET_REQUEST(QueryPointer,          query_pointer)
	GET_REQUEST(RandrGetProviders,     randr_get_providers)
	GET_REQUEST(RandrGetProviderInfo,  randr_get_provider_info)
	GET_REQUEST(RandrGetOutputInfo,    randr_get_output_info)
	GET_REQUEST(RandrGetCrtcInfo,      randr_get_crtc_info)
	GET_REQUEST(RandrGetOutputPrimary, randr_get_output_primary)


	// Setters
	SET_REQUEST(ConfigureWindow,        configure_window)
	SET_REQUEST(ChangeWindowAttributes, change_window_attributes)
	SET_REQUEST(SetInputFocus,          set_input_focus)
	SET_REQUEST(MapWindow,              map_window)
	SET_REQUEST(UnmapWindow,            unmap_window)



	#undef GET_REQUEST
	#undef SET_REQUEST





	// call get on multiple args
	template <typename T1, typename T2, typename... Ts>
	auto get(fluke::Connection& conn, T1&& arg1, T2&& arg2, Ts&&... args) {
		// this is kind of messy but required to not conflict with the overloaded variants defined above
		return std::tuple{
			fluke::get(conn, std::forward<T1>(arg1)),
			fluke::get(conn, std::forward<T2>(arg2)),
			fluke::get(conn, std::forward<Ts>(args))...
		};
	}






	// Getter functions
	InternAtomCookie intern_atom(fluke::Connection& conn, bool only_if_exists, std::string_view name) {
		return xcb_intern_atom_unchecked(conn, only_if_exists, name.size(), name.data());
	}


	GetWindowAttributesCookie get_window_attributes(fluke::Connection& conn, xcb_window_t win) {
		return xcb_get_window_attributes_unchecked(conn, win);
	}


	GetGeometryCookie get_geometry(fluke::Connection& conn, xcb_drawable_t draw) {
		return xcb_get_geometry_unchecked(conn, draw);
	}


	GetPropertyCookie get_property(fluke::Connection& conn, bool delete_, xcb_window_t win, xcb_atom_t property, xcb_atom_t type, uint32_t long_offset, uint32_t long_length) {
		return xcb_get_property_unchecked(conn, delete_, win, property, type, long_offset, long_length);
	}


	GetInputFocusCookie get_input_focus(fluke::Connection& conn) {
		return xcb_get_input_focus_unchecked(conn);
	}


	QueryTreeCookie query_tree(fluke::Connection& conn, xcb_window_t win) {
		return xcb_query_tree_unchecked(conn, win);
	}


	QueryPointerCookie query_pointer(fluke::Connection& conn, xcb_window_t win) {
		return xcb_query_pointer_unchecked(conn, win);
	}






	RandrGetProvidersCookie randr_get_providers(fluke::Connection& conn, xcb_window_t win) {
		return xcb_randr_get_providers_unchecked(conn, win);
	}


	RandrGetProviderInfoCookie randr_get_provider_info(fluke::Connection& conn, xcb_randr_provider_t provider) {
		return xcb_randr_get_provider_info_unchecked(conn, provider, XCB_CURRENT_TIME);
	}


	RandrGetOutputInfoCookie randr_get_output_info(fluke::Connection& conn, xcb_randr_provider_t provider) {
		return xcb_randr_get_output_info_unchecked(conn, provider, XCB_CURRENT_TIME);
	}


	RandrGetCrtcInfoCookie randr_get_crtc_info(fluke::Connection& conn, xcb_randr_crtc_t crtc) {
		return xcb_randr_get_crtc_info_unchecked(conn, crtc, XCB_CURRENT_TIME);
	}


	RandrGetOutputPrimaryCookie randr_get_output_primary(fluke::Connection& conn, xcb_window_t win) {
		return xcb_randr_get_output_primary_unchecked(conn, win);
	}






	// Setter functions
	template <typename T, typename... Ts>
	ConfigureWindowCookie configure_window(fluke::Connection& conn, xcb_window_t win, uint16_t value_mask, T&& arg, Ts&&... args) {
		uint32_t values[] = { std::forward<T>(arg), std::forward<Ts>(args)... };
		return xcb_configure_window(conn, win, value_mask, values);
	}

	ConfigureWindowCookie configure_window(fluke::Connection& conn, xcb_window_t win, uint16_t value_mask, uint32_t* args) {
		return xcb_configure_window(conn, win, value_mask, args);
	}





	template <typename T, typename... Ts>
	ChangeWindowAttributesCookie change_window_attributes(fluke::Connection& conn, xcb_window_t win, uint16_t value_mask, T&& arg, Ts&&... args) {
		uint32_t values[] = { std::forward<T>(arg), std::forward<Ts>(args)... };
		return xcb_change_window_attributes(conn, win, value_mask, values);
	}

	ChangeWindowAttributesCookie change_window_attributes(fluke::Connection& conn, xcb_window_t win, uint16_t value_mask, uint32_t* args) {
		return xcb_change_window_attributes(conn, win, value_mask, args);
	}






	SetInputFocusCookie set_input_focus(fluke::Connection& conn, uint8_t revert_to, xcb_window_t focus) {
		return xcb_set_input_focus(conn, revert_to, focus, XCB_CURRENT_TIME);
	}


	MapWindowCookie map_window(fluke::Connection& conn, xcb_window_t win) {
		return xcb_map_window(conn, win);
	}


	UnmapWindowCookie unmap_window(fluke::Connection& conn, xcb_window_t win) {
		return xcb_unmap_window(conn, win);
	}
}

