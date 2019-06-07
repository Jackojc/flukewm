#pragma once

#include <memory>
#include <cstdlib>
#include <util/xcb.hpp>
#include <util/exception.hpp>
#include <util/conversions.hpp>
#include <structures/connection.hpp>

namespace fluke {

	namespace detail {
		void geom_cleanup(xcb_get_geometry_reply_t* geom) noexcept {
			std::free(geom);
		}

		void attr_cleanup(xcb_get_window_attributes_reply_t* attr) noexcept {
			std::free(attr);
		}
	}


	struct Attributes {

		struct Data {
			int16_t x, y;
			uint16_t width, height;
			uint16_t border_width;
			bool is_mapped, is_ignored;
			xcb_window_t root;
		};

		fluke::Connection conn;
		xcb_window_t win;
		Data data;

		xcb_get_geometry_cookie_t geom_cookie;
		std::unique_ptr<xcb_get_geometry_reply_t, decltype(&detail::geom_cleanup)> geom_reply;

		xcb_get_window_attributes_cookie_t attr_cookie;
		std::unique_ptr<xcb_get_window_attributes_reply_t, decltype(&detail::attr_cleanup)> attr_reply;


		Attributes(const fluke::Connection& c, xcb_window_t w)
			: conn(c), win(w),

			  geom_cookie(xcb_get_geometry(conn, w)),
			  geom_reply(nullptr, detail::geom_cleanup),

			  attr_cookie(xcb_get_window_attributes(conn, w)),
			  attr_reply(nullptr, detail::attr_cleanup)
		{

		}


		Data get() {
			if (not geom_reply) {
				xcb_generic_error_t* error;
				geom_reply.reset(xcb_get_geometry_reply(conn, geom_cookie, &error));

				if (not geom_reply) {
					std::free(error);
					throw fluke::except::GeometryError("could not get geometry for '", fluke::to_hex(win), "'!");
				}

				std::free(error);

				data.x = geom_reply->x;
				data.y = geom_reply->y;
				data.width = geom_reply->width;
				data.height = geom_reply->height;
				data.border_width = geom_reply->border_width;
			}


			if (not attr_reply) {
				xcb_generic_error_t* error;
				attr_reply.reset(xcb_get_window_attributes_reply(conn, attr_cookie, &error));

				if (not attr_reply) {
					std::free(error);
					throw fluke::except::AttributeError("could not get geometry for '", fluke::to_hex(win), "'!");
				}

				std::free(error);

				data.is_mapped = attr_reply->map_state == XCB_MAP_STATE_VIEWABLE;
				data.is_ignored = bool(attr_reply->override_redirect);
			}

			return data;
		}


		Data* operator->() {
			get();
			return &data;
		}

	};

}
