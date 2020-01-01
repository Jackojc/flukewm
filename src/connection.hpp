#pragma once

#include <memory>

#include <fluke.hpp>

namespace fluke {

	namespace detail {
		void cleanup_connection(xcb_connection_t* conn) {
			xcb_disconnect(conn);
		}
	}

	class Connection {
		// Data
		private:
			std::unique_ptr<xcb_connection_t, decltype(&detail::cleanup_connection)> conn;
			xcb_screen_t* scrn;


		// Constructor
		public:
			Connection():
				conn(xcb_connect(nullptr, nullptr), &detail::cleanup_connection),
				scrn(xcb_setup_roots_iterator(xcb_get_setup(conn.get())).data)
			{
				if (xcb_connection_has_error(conn.get()))
					throw fluke::ConnectionError();

				if (not scrn)
					throw fluke::ScreenError();
			}


		// Implicit cast operators
		public:
			operator xcb_connection_t*() const {
				return conn.get();
			}

			operator xcb_screen_t*() const {
				return scrn;
			}


		// Functions
		public:
			xcb_connection_t* get() const {
				return conn.get();
			}

			constexpr xcb_window_t root() const {
				return scrn->root;
			}

			constexpr xcb_screen_t* screen() const {
				return scrn;
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
