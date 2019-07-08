#pragma once

#include <memory>
#include <util/xcb.hpp>
#include <util/exception.hpp>

namespace fluke {

	struct Connection {
		std::shared_ptr<xcb_connection_t> conn;
		xcb_screen_t* screen;

		Connection()
			: conn(xcb_connect(nullptr, nullptr), [] (auto c) { xcb_disconnect(c); }),
			  screen(xcb_setup_roots_iterator(xcb_get_setup(conn.get())).data)
		{
			if (xcb_connection_has_error(conn.get()))
				throw fluke::ConnectionError();

			if (not screen)
				throw fluke::ScreenError();
		}

		operator xcb_connection_t* () const {
			return conn.get();
		}

		operator xcb_screen_t* () const {
			return screen;
		}

		constexpr xcb_window_t root() const {
			return screen->root;
		}

		// Flush all pending requests.
		void flush() noexcept {
			xcb_flush(conn.get());
		}

		// Flush all pending requests _and_ wait for them to complete.
		void sync() noexcept {
			xcb_aux_sync(conn.get());
		}
	};

}
