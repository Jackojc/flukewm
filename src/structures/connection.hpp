#pragma once

#include <memory>
#include <util/xcb.hpp>
#include <util/exception.hpp>

namespace fluke {

	struct Connection {
		std::shared_ptr<xcb_connection_t> conn;

		Connection()
			: conn(xcb_connect(nullptr, nullptr), [] (auto c) { xcb_disconnect(c); })
		{
			if (xcb_connection_has_error(conn.get()))
				throw fluke::ConnectionError();
		}

		operator xcb_connection_t* () const {
			return conn.get();
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
