#include <iostream>
#include <vector>

#include <fluke.hpp>

int main(int argc, const char** argv) {
	fluke::Connection conn;

	// xcb_window_t win = std::strtoul("0x05800006", nullptr, 16);


	// fluke::chwb(conn, 1, std::strtoul("ff00ff", nullptr, 16), win);
	// fluke::chwn(conn, win, "firefucks");
	// fluke::chwf(conn, win);
	// fluke::killw(conn, win);
	// fluke::ignw(conn, win, 1);
	// fluke::wtp(conn, win, 10, 10, 300, 300);
	// fluke::chwso(conn, win, fluke::STACK_BELOW);
	// fluke::mapw(conn, win, fluke::MAP_TOGGLE);
	// fluke::killwa(conn, win);
	// fluke::wmp(conn, conn, 5, 5, fluke::MOUSE_RELATIVE);
	// fluke::wrs(conn, win, 10, 10, fluke::RESIZE_GROW);
	// fluke::wmv(conn, win, -10, 10);

	// std::cout << fluke::wname(conn, win) << '\n';
	// std::cout << fluke::pfw(conn) << '\n';
	// std::cout << fluke::ppd(conn) << '\n';
	// std::cout << fluke::phd(conn, conn) << '\n';
	// std::cout << fluke::dattr(conn, conn, "LVDS1") << '\n';
	// std::cout << fluke::pfd(conn, conn) << '\n';
	// std::cout << fluke::wattr(conn, win) << '\n';

	// for (const auto& w: fluke::lsw(conn, conn))
	// 	std::cout << w << '\n';

	// for (const auto& disp: fluke::lsd(conn, conn))
	// 	std::cout << disp << '\n';

	fluke::window::resize::interactive(conn, fluke::get_focused_window(conn));

	return 0;
}
