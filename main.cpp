#include <iostream>
#include <vector>


#include <modules/chwb.hpp>


static xcb_connection_t *conn;





int main(int argc, const char** argv) {
	init_xcb(&conn);

	xcb_window_t win = std::strtoul("0x02c00006", nullptr, 16);


	fluke::chwb(conn, 1, std::strtoul("ff00ff", nullptr, 16), win);


	xcb_aux_sync(conn);
	kill_xcb(&conn);

	return 0;
}
