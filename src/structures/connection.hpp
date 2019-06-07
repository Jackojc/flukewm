#pragma once

#include <memory>
#include <cstdlib>
#include <util/xcb.hpp>
#include <util/exception.hpp>

namespace fluke {

	namespace detail {
		void xcb_cleanup(xcb_connection_t* conn) {
			if (not conn)
				return;

			xcb_aux_sync(conn); // blocking call to sync all operations.
			xcb_disconnect(conn);  // calls free.
		}
	}


	struct Connection {

		std::shared_ptr<xcb_connection_t> conn;


		Connection()
			: conn(xcb_connect(nullptr, nullptr), detail::xcb_cleanup)
		{
			if (xcb_connection_has_error(conn.get()))
				throw fluke::except::ConnectionError();
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


		xcb_atom_t get_atom(const std::string& str) {
			auto cookie = xcb_intern_atom(*this, 0, str.size(), str.c_str());
			auto reply  = std::unique_ptr<xcb_intern_atom_reply_t>(xcb_intern_atom_reply(*this, cookie, nullptr));

			if (not reply)
				return XCB_ATOM_STRING;

			return reply->atom;
		}

	};

}
