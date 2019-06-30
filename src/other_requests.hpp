#pragma once


#include <utility>

#include <util/exception.hpp>
#include <util/tags.hpp>
#include <util/xcb.hpp>

#include <structures/connection.hpp>
#include <structures/common.hpp>

#include <requests.hpp>


namespace fluke {


	// Just wraps the GetGeometry request type for convenience.
	struct GetWindowRect: GetGeometry {
		template <typename... Ts>
		constexpr GetWindowRect(Ts&&... args):
			GetGeometry::GetGeometry(std::forward<Ts>(args)...) {}

		fluke::Rect get() const {
			auto ret = GetGeometry::get();
			return { ret->x, ret->y, ret->width, ret->height };
		}
	};









	namespace detail {
		using SetRequestConfigure = Request<
			detail::SetterTag, xcb_void_cookie_t, xcb_generic_error_t, fluke::ConfigureWindowError
		>;
	}


	// Setters
	struct SetWindowPos: detail::SetRequestConfigure {
		template <typename... Ts>
		constexpr SetWindowPos(const fluke::Connection& conn_, xcb_window_t win, const fluke::Position& p):
			Request::Request(conn_, xcb_configure_window_checked(
				conn_, win, fluke::CONFIG_WINDOW_POS, fluke::to_uint32_array(p).data()
			)) {}

		auto get() const { return Request::get(xcb_request_check); }
	};




	struct SetWindowSize: detail::SetRequestConfigure {
		template <typename... Ts>
		constexpr SetWindowSize(const fluke::Connection& conn_, xcb_window_t win, const fluke::Size& p):
			Request::Request(conn_, xcb_configure_window_checked(
				conn_, win, fluke::CONFIG_WINDOW_SIZE, fluke::to_uint32_array(p).data()
			)) {}

		auto get() const { return Request::get(xcb_request_check); }
	};




	struct SetWindowRect: detail::SetRequestConfigure {
		template <typename... Ts>
		constexpr SetWindowRect(const fluke::Connection& conn_, xcb_window_t win, const fluke::Rect& p):
			Request::Request(conn_, xcb_configure_window_checked(
				conn_, win, fluke::CONFIG_WINDOW_RECT, fluke::to_uint32_array(p).data()
			)) {}

		auto get() const { return Request::get(xcb_request_check); }
	};



}
