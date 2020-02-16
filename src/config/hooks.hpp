#pragma once


// User hooks
namespace fluke {
	// Hooks called on launch and exit.
	inline void on_launch(fluke::Connection&) {
		fluke::exec("xsetroot", "-cursor_name", "left_ptr");
		fluke::exec("keyboard_set");
		fluke::exec("run_once","picom");
		fluke::exec("run_once","pulseaudio", "--start");
		fluke::exec("run_once","dunst");
		fluke::exec("autorandr", "-c");
		fluke::exec("wallpaper_random");
	}

	inline void on_exit(fluke::Connection&) {}


	// These hooks are called when window focus is changed.
	inline void on_focus_in(fluke::Connection&, const fluke::FocusInEvent&) {}
	inline void on_focus_out(fluke::Connection&, const fluke::FocusOutEvent&) {}


	// These hooks are called when the mouse enters/leaves a window.
	inline void on_hover_in(fluke::Connection&, const fluke::EnterNotifyEvent&) {}
	inline void on_hover_out(fluke::Connection&, const fluke::LeaveNotifyEvent&) {}


	// Called on window creation or destruction.
	inline void on_create(fluke::Connection&, const fluke::CreateNotifyEvent&) {}
	inline void on_destroy(fluke::Connection&, const fluke::DestroyNotifyEvent&) {}


	// Called when windows are mapped or unmapped.
	inline void on_map(fluke::Connection&, const fluke::MapRequestEvent&) {}
	inline void on_unmap(fluke::Connection&, const fluke::UnmapNotifyEvent&) {}


	// This is called when a window asks to be configured.
	inline void on_configure(fluke::Connection&, const fluke::ConfigureRequestEvent&) {}


	// This is called when the pointer is moved.
	inline void on_motion(fluke::Connection&, const fluke::MotionNotifyEvent&) {}


	// This is called when a grabbed key is pressed.
	inline void on_keypress(fluke::Connection&, const fluke::KeyPressEvent&) {}


	// Called when an error occurs.
	inline void on_error(fluke::Connection&, const fluke::Error&) {}


	// These are usually called when anything EWMH happens.
	inline void on_property(fluke::Connection&, const fluke::PropertyNotifyEvent&) {}
	inline void on_client_message(fluke::Connection&, const fluke::ClientMessageEvent&) {}


	// Randr/screen hooks.
	inline void on_randr_screen_change(fluke::Connection&, const fluke::RandrScreenChangeNotifyEvent&) {
		fluke::exec("autorandr", "-c");
	}

	inline void on_randr_notify(fluke::Connection&, const fluke::RandrNotifyEvent&) {}
}

