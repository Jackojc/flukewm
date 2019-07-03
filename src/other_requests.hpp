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
	struct GetWindowRect: fluke::GetWindowGeometry {
		constexpr GetWindowRect(const fluke::Connection& conn_, xcb_window_t win):
			GetWindowGeometry::GetWindowGeometry(conn_, win) {}

		fluke::Rect get() const {
			auto ret = GetWindowGeometry::get();
			return { ret->x, ret->y, ret->width, ret->height };
		}
	};


	struct GetPointerPos: fluke::GetPointer {
		constexpr GetPointerPos(const fluke::Connection& conn_):
			GetPointer::GetPointer(conn_, conn_.root()) {}

		fluke::Position get() const {
			auto ret = GetPointer::get();
			return { ret->root_x, ret->root_y };
		}
	};





	// Some convenience wrappers.
	struct SetWindowPos: fluke::SetWindowConfig {
		constexpr SetWindowPos(
			const fluke::Connection& conn_, xcb_window_t win, const fluke::Position& p
		):
			SetWindowConfig::SetWindowConfig(
				conn_, win, fluke::CONFIG_WINDOW_POS, fluke::to_uint32_array(p).data()
			)
		{}

		auto get() const { return SetWindowConfig::get(); }
	};




	struct SetWindowSize: fluke::SetWindowConfig {
		constexpr SetWindowSize(
			const fluke::Connection& conn_, xcb_window_t win, const fluke::Size& p
		):
			SetWindowConfig::SetWindowConfig(
				conn_, win, fluke::CONFIG_WINDOW_SIZE, fluke::to_uint32_array(p).data()
			)
		{}

		auto get() const { return SetWindowConfig::get(); }
	};




	struct SetWindowRect: fluke::SetWindowConfig {
		constexpr SetWindowRect(
			const fluke::Connection& conn_, xcb_window_t win, const fluke::Rect& p
		):
			SetWindowConfig::SetWindowConfig(
				conn_, win, fluke::CONFIG_WINDOW_RECT, fluke::to_uint32_array(p).data()
			)
		{}

		auto get() const { return SetWindowConfig::get(); }
	};



}
