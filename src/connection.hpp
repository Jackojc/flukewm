#pragma once

#include <memory>

#include <fluke.hpp>

namespace fluke {
	class Connection {
		// Data
		private:
			std::shared_ptr<xcb_connection_t> conn;
			xcb_screen_t* screen;


		// Constructor
		public:
			Connection():
				conn(xcb_connect(nullptr, nullptr), [] (auto c) { xcb_disconnect(c); }),
				screen(xcb_setup_roots_iterator(xcb_get_setup(conn.get())).data)
			{
				if (xcb_connection_has_error(conn.get()))
					throw fluke::ConnectionError();

				if (not screen)
					throw fluke::ScreenError();
			}


			Connection(const Connection& other) {
				conn = other.conn;
				screen = other.screen;
			}


			Connection& operator=(const Connection& other) {
				if (this != &other) {
					conn = other.conn;
					screen = other.screen;
				}

				return *this;
			}


		// Implicit cast operators
		public:
			operator xcb_connection_t* () const {
				return conn.get();
			}

			operator xcb_screen_t* () const {
				return screen;
			}


		// Functions
		public:
			xcb_connection_t* get() const {
				return conn.get();
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
