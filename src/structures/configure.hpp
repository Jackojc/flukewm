#pragma once

#include <memory>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <cstdint>
#include <util/xcb.hpp>
#include <util/exception.hpp>
#include <util/conversions.hpp>
#include <structures/connection.hpp>
#include <structures/colour.hpp>

namespace fluke {

	struct Configure {

		struct Element {
			uint8_t priority;
			uint32_t val;

			Element(uint8_t p, uint32_t v)
				: priority(p), val(v) {}
		};


		fluke::Connection conn;
		xcb_window_t win;

		uint16_t mask = 0;
		uint16_t mask_attr = 0;

		std::vector<Element> values;
		std::vector<Element> values_attr;


		Configure(const fluke::Connection& c, xcb_window_t w)
			: conn(c), win(w)
		{
			values.reserve(7);
			values_attr.reserve(7);
		}


		fluke::Configure& x(int16_t x) noexcept {
			values.emplace_back(0, x);
			mask |= XCB_CONFIG_WINDOW_X;
			return *this;
		}


		fluke::Configure& y(int16_t y) noexcept {
			values.emplace_back(1, y);
			mask |= XCB_CONFIG_WINDOW_Y;
			return *this;
		}


		fluke::Configure& pos(int16_t x, int16_t y) noexcept {
			values.emplace_back(0, x);
			values.emplace_back(1, y);
			mask |= XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y;
			return *this;
		}


		fluke::Configure& width(uint16_t width) noexcept {
			values.emplace_back(2, width);
			mask |= XCB_CONFIG_WINDOW_WIDTH;
			return *this;
		}


		fluke::Configure& height(uint16_t height) noexcept {
			values.emplace_back(3, height);
			mask |= XCB_CONFIG_WINDOW_HEIGHT;
			return *this;
		}


		fluke::Configure& size(uint16_t width, uint16_t height) noexcept {
			values.emplace_back(2, width);
			values.emplace_back(3, height);
			mask |= XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT;
			return *this;
		}


		fluke::Configure& border_size(uint16_t size) noexcept {
			values.emplace_back(4, size);
			mask |= XCB_CONFIG_WINDOW_BORDER_WIDTH;
			return *this;
		}


		fluke::Configure& border_colour(const std::string& hex) noexcept {
			values_attr.emplace_back(0, std::strtoul(hex.c_str(), nullptr, 16));
			mask_attr |= XCB_CW_BORDER_PIXEL;
			return *this;
		}


		fluke::Configure& border_colour(const fluke::Colour& col) noexcept {
			values_attr.emplace_back(0, *reinterpret_cast<const uint32_t*>(&col));
			mask_attr |= XCB_CW_BORDER_PIXEL;
			return *this;
		}


		fluke::Configure& layer(uint32_t layer) noexcept {
			values.emplace_back(5, layer);
			mask|= XCB_CONFIG_WINDOW_STACK_MODE;
			return *this;
		}


		fluke::Configure& commit() {
			std::sort(values.begin(), values.end(), [] (const auto& a, const auto& b) {
				return a.priority < b.priority;
			});

			std::sort(values_attr.begin(), values_attr.end(), [] (const auto& a, const auto& b) {
				return a.priority < b.priority;
			});


			std::vector<uint32_t> values2;
			for (const auto& x: values)
				values2.emplace_back(x.val);

			std::vector<uint32_t> values_attr2;
			for (const auto& x: values_attr)
				values_attr2.emplace_back(x.val);


			auto cookie1 = xcb_configure_window_checked(conn, win, mask, values2.data());
			auto cookie2 = xcb_change_window_attributes_checked(conn, win, mask_attr, values_attr2.data());

			auto error1 = std::unique_ptr<xcb_generic_error_t>(xcb_request_check(conn, cookie1));
			auto error2 = std::unique_ptr<xcb_generic_error_t>(xcb_request_check(conn, cookie2));


			if (error1 or error2)
				throw fluke::except::ConfigureError("could not configure '", fluke::to_hex(win), "'!");


			mask = 0;
			mask_attr = 0;

			values.clear();
			values_attr.clear();

			conn.sync();
			return *this;
		}

	};

}
