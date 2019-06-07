#include <iostream>
#include <vector>

#include <fluke.hpp>

int main(int argc, const char** argv) {
	try {
		fluke::Connection conn;
		fluke::Window win{conn, static_cast<xcb_window_t>(std::strtoul("0x03400006", nullptr, 16))};

		// {
		// 	auto [x, y, w, h, b, m, i, r] = win.attr().get();
		// 	std::cout << w << ", " << h << '\n';
		// }

		// auto conf = win.conf().size(300, 300).border_colour(fluke::Colour{255, 0, 255, 255}).pos(0, 0).commit();

		// {
		// 	auto [x, y, w, h, b, m, i, r] = win.attr().get();
		// 	std::cout << w << ", " << h << '\n';
		// }

		// win.close();

		win.shrink(10);

	} catch (const std::exception& e) {
		fluke::errorln_em("error: ", e.what());
	}

	return 0;
}
