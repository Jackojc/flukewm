#pragma once

#include <memory>
#include <fluke.hpp>


namespace fluke {

	namespace detail {
		void cleanup_connection(xcb_connection_t* conn) noexcept {
			xcb_disconnect(conn);
		}
	}

	class Connection {
		// Data
		private:
			std::unique_ptr<xcb_connection_t, decltype(&detail::cleanup_connection)> conn;
			std::unique_ptr<xcb_key_symbols_t, decltype(&xcb_key_symbols_free)> key_symbols;

			xcb_screen_t* scrn;


		// Constructor
		public:
			Connection():
				conn(xcb_connect(nullptr, nullptr), &detail::cleanup_connection),
				key_symbols(xcb_key_symbols_alloc(conn.get()), &xcb_key_symbols_free),
				scrn(xcb_setup_roots_iterator(xcb_get_setup(conn.get())).data)
			{

			}


		// Implicit cast operators
		public:
			operator xcb_connection_t*() const {
				return conn.get();
			}


		// Functions
		public:
			xcb_connection_t* get() const {
				return conn.get();
			}

			xcb_key_symbols_t* keysyms() const {
				return key_symbols.get();
			}

			constexpr xcb_window_t root() const noexcept {
				return scrn->root;
			}

			constexpr xcb_screen_t* screen() const noexcept {
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
