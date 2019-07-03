#pragma once

#include <type_traits>
#include <memory>
#include <cstdlib>
#include <util/exception.hpp>
#include <util/tags.hpp>
#include <util/xcb.hpp>
#include <structures/connection.hpp>

namespace fluke {

	template <typename T, typename C, typename R, typename Err>
	struct Request {
		using reply_t = std::unique_ptr<R, decltype(&std::free)>;
		using tag_t   = T;

		fluke::Connection conn;
		C cookie;

		Request(const fluke::Connection& conn_, C cookie_)
			: conn(conn_), cookie(cookie_) {}

		template <typename F>
		auto get(F func) const {
			if constexpr(std::is_same_v<tag_t, detail::GetterTag>) {
				if (auto ret = reply_t{func(conn, cookie, nullptr), std::free}; ret)
					return ret;

			} else if constexpr(std::is_same_v<tag_t, detail::SetterTag>) {
				if (auto ret = reply_t{func(conn, cookie), std::free}; not ret)
					return ret;

			} else {
				throw fluke::UnknownTagError();
			}

			throw Err();
		}
	};


	// Define a get request, nothing special.
	#define GET_REQUEST(name, type) \
		struct name: Request<detail::GetterTag, xcb_##type##_cookie_t, xcb_##type##_reply_t, fluke::name##Error> { \
			template <typename... Ts> constexpr name(const fluke::Connection& conn_, Ts&&... args): \
				Request::Request(conn_, xcb_##type(conn_, std::forward<Ts>(args)...)) {} \
			auto get() const { return Request::get(xcb_##type##_reply); } \
		};


	// Define a set request, uses the same cookie and reply function for all setters.
	#define SET_REQUEST(name, type) \
		struct name: Request<detail::SetterTag, xcb_void_cookie_t, xcb_generic_error_t, fluke::name##Error> { \
			template <typename... Ts> constexpr name(const fluke::Connection& conn_, Ts&&... args): \
				Request::Request(conn_, xcb_##type##_checked(conn_, std::forward<Ts>(args)...)) {} \
			auto get() const { return Request::get(xcb_request_check); } \
		};


	// Getters
	GET_REQUEST(GetInternAtom,         intern_atom)
	GET_REQUEST(GetWindowAttributes,   get_window_attributes)
	GET_REQUEST(GetWindowGeometry,     get_geometry)
	GET_REQUEST(GetWindowProperty,     get_property)
	GET_REQUEST(GetInputFocus,         get_input_focus)
	GET_REQUEST(GetTree,               query_tree)
	GET_REQUEST(GetPointer,            query_pointer)
	GET_REQUEST(GetRandrProviders,     randr_get_providers)
	GET_REQUEST(GetRandrProviderInfo,  randr_get_provider_info)
	GET_REQUEST(GetRandrOutputInfo,    randr_get_output_info)
	GET_REQUEST(GetRandrCrtcInfo,      randr_get_crtc_info)
	GET_REQUEST(GetRandrOutputPrimary, randr_get_output_primary)


	// // Setters
	SET_REQUEST(SetWindowConfig,     configure_window)
	SET_REQUEST(SetWindowAttributes, change_window_attributes)


	#undef GET_REQUEST
	#undef SET_REQUEST

}
