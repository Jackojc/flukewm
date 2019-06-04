#include <iostream>
#include <vector>

#include <utils/util.hpp>
#include <structures/connection.hpp>
#include <chwb.hpp>
#include <chwn.hpp>
#include <chwf.hpp>
#include <ppd.hpp>



int main(int argc, const char** argv) {
	fluke::Connection conn;

	xcb_window_t win = std::strtoul("0x04200006", nullptr, 16);


	// fluke::chwb(conn, 1, std::strtoul("ff00ff", nullptr, 16), win);
	// fluke::chwn(conn, win, "firefucks");
	fluke::chwf(conn, win);


	// std::cout << fluke::ppd(conn) << '\n';


	return 0;
}
