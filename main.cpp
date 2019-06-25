#include <iostream>
#include <vector>
#include <cstdlib>

#include <tinge.hpp>
#include <fluke.hpp>

int main(int argc, const char* argv[]) {
	if (argc < 2) {
		tinge::errorln_em("error: ", "no window specified!");
		return 1;
	}



	/*
		todo:
			- If requestbuffer only has a single return value, unpack the tuple internally
			  so you can just do `auto x = ...` instead of `auto&& [x] = ...`.
			- Make the specialised requests inherit from ConfigureWindow etc to cut down
			  on code duplication.
	*/


	fluke::Connection conn;
	xcb_window_t win = std::strtoul(argv[1], nullptr, 16);


	try {
		auto&& [rect] = fluke::RequestBuffer{
			fluke::SetWindowRect(conn, win, {0, 0, 1366, 768}),
			fluke::GetWindowRect(conn, win)
		}.get();


		std::cerr << rect.x << ", " << rect.y << ", " << rect.w << ", " << rect.h << '\n';

		conn.sync();

		rect = fluke::GetWindowRect(conn, win).get();
		std::cerr << rect.x << ", " << rect.y << ", " << rect.w << ", " << rect.h << '\n';


	} catch (const std::exception& e) {
		tinge::errorln_em("error: ", e.what());
		return 1;
	}


	return 0;
}
