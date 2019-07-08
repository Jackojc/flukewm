#include <iostream>
#include <vector>
#include <cstdlib>

#include <tinge.hpp>
#include <fluke.hpp>

int main(int argc, const char* argv[]) {
	// if (argc < 2) {
	// 	tinge::errorln_em("error: ", "no window specified!");
	// 	return 1;
	// }



	/*
		todo:
			- Make the specialised requests inherit from ConfigureWindow etc to cut down
			  on code duplication.


			// implement these:

			GetWindowName
			SetWindowName

			[x] GetPointerPos
			SetPointerPos

			GetDisplayPos           = Get display coords
			GetDisplaySize          = Get display resolution
			GetDisplayRect          = Get display coords and resolution

			SetWindowFocus          = Change window focus
			GetWindowFocus          = Get focused window

			SetWindowVisibility     = map/unmap a window
			GetWindowVisibility     = check mapped status

			SetWindowTracked        = ignore/notice a window
			GetWindowTracked        = checked ignored status

			KillWindow              = force close a window
			CloseWindow             = close a window gracefully

			SetWindowStackingOrder
			GetWindowStackingOrder

			SetWindowBorderColour
			SetWindowBorderSize
			GetWindowBorderColour
			GetWindowBorderSize

			ListWindows
			ListDisplays

			GetDisplayFocus
			SetDisplayFocus

			GetPrimaryDisplay
			SetPrimaryDisplay

			GetHoveredWindow        = get window with mouse on top
			GetHoveredDisplay       = get display mouse is on
	*/


	fluke::Connection conn;
	// xcb_window_t win = std::strtoul(argv[1], nullptr, 16);


	try {
		auto pos = fluke::RequestBuffer{
			fluke::GetPointerPos(conn)
			// fluke::SetWindowRect(conn, win, {0, 0, 1364, 766}),
			// fluke::GetWindowRect(conn, win)
		}.get();


		std::cerr << pos.x << ", " << pos.y << '\n';

		// std::cerr << rect.x << ", " << rect.y << ", " << rect.w << ", " << rect.h << '\n';

		// conn.sync();

		// rect = fluke::GetWindowRect(conn, win).get();
		// std::cerr << rect.x << ", " << rect.y << ", " << rect.w << ", " << rect.h << '\n';


	} catch (const std::exception& e) {
		tinge::errorln_em("error: ", e.what());
		return 1;
	}


	return 0;
}
