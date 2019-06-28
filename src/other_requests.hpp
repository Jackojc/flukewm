#pragma once

#include <util/exception.hpp>
#include <util/tags.hpp>
#include <util/xcb.hpp>
#include <structures/connection.hpp>
#include <structures/common.hpp>
#include <requests.hpp>

namespace fluke {

	// Getters.
	struct GetWindowPos: Request<
		detail::GetterTag, xcb_get_geometry_cookie_t, xcb_get_geometry_reply_t, fluke::GetGeometryError
	> {
		template <typename... Ts> constexpr GetWindowPos(const fluke::Connection& conn_, Ts&&... args):
			Request::Request(conn_, xcb_get_geometry(conn_, std::forward<Ts>(args)...)) {}

		fluke::WindowPosition get() const {
			auto ret = Request::get(xcb_get_geometry_reply);
			return { ret->x, ret->y };
		}
	};



	struct GetWindowSize: Request<
		detail::GetterTag, xcb_get_geometry_cookie_t, xcb_get_geometry_reply_t, fluke::GetGeometryError
	> {
		template <typename... Ts> constexpr GetWindowSize(const fluke::Connection& conn_, Ts&&... args):
			Request::Request(conn_, xcb_get_geometry(conn_, std::forward<Ts>(args)...)) {}

		fluke::WindowSize get() const {
			auto ret = Request::get(xcb_get_geometry_reply);
			return { ret->width, ret->height };
		}
	};


	struct GetWindowRect: Request<
		detail::GetterTag, xcb_get_geometry_cookie_t, xcb_get_geometry_reply_t, fluke::GetGeometryError
	> {
		template <typename... Ts> constexpr GetWindowRect(const fluke::Connection& conn_, Ts&&... args):
			Request::Request(conn_, xcb_get_geometry(conn_, std::forward<Ts>(args)...)) {}

		fluke::WindowRect get() const {
			auto ret = Request::get(xcb_get_geometry_reply);
			return { ret->x, ret->y, ret->width, ret->height };
		}
	};











	// Setters
	struct SetWindowPos: Request<
		detail::SetterTag, xcb_void_cookie_t, xcb_generic_error_t, fluke::ConfigureWindowError
	> {
		template <typename... Ts>
		constexpr SetWindowPos(const fluke::Connection& conn_, xcb_window_t win, const fluke::WindowPos& p):
			Request::Request(conn_, xcb_configure_window(conn_, win, fluke::CONFIG_WINDOW_POS, p))
		{}

		auto get() const {
			return Request::get(xcb_request_check);
		}
	};




	struct SetWindowSize: Request<
		detail::SetterTag, xcb_void_cookie_t, xcb_generic_error_t, fluke::ConfigureWindowError
	> {
		template <typename... Ts>
		constexpr SetWindowSize(const fluke::Connection& conn_, xcb_window_t win, const fluke::WindowSize& p):
			Request::Request(conn_, xcb_configure_window_checked(conn_, win, fluke::CONFIG_WINDOW_SIZE, p))
		{}

		auto get() const {
			return Request::get(xcb_request_check);
		}
	};




	struct SetWindowRect: Request<
		detail::SetterTag, xcb_void_cookie_t, xcb_generic_error_t, fluke::ConfigureWindowError
	> {
		struct WindowPos {
			uint32_t x, y, w, h;
		};

		template <typename... Ts>
		constexpr SetWindowRect(const fluke::Connection& conn_, xcb_window_t win, const WindowPos& p):
			Request::Request(conn_, xcb_configure_window_checked(
				conn_, win,
				XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y | XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT,
				&p
			))
		{}

		auto get() const {
			return Request::get(xcb_request_check);
		}
	};



}
