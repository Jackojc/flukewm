#pragma once

#include <utils/core.hpp>

namespace fluke {
	struct Connection {
		xcb_connection_t* conn;


		Connection()
			: conn(nullptr)
		{
			fluke::init_xcb(&conn);
		}


		~Connection() {
			xcb_aux_sync(conn);
			fluke::kill_xcb(&conn);
		}


		operator xcb_connection_t*() {
			return conn;
		}
	};
}
