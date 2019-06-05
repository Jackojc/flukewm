#pragma once

#include <utils/core.hpp>

namespace fluke {
	struct Connection {
		xcb_connection_t* conn;
		xcb_screen_t* screen;


		Connection()
			: conn(nullptr), screen(nullptr)
		{
			fluke::init_xcb(&conn);
			screen = fluke::get_screen(conn);
		}


		~Connection() {
			fluke::kill_xcb(&conn);
		}


		operator xcb_connection_t*() const {
			return conn;
		}

		operator xcb_screen_t*() const {
			return screen;
		}

	};
}
