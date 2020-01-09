#pragma once

#include <fluke.hpp>

namespace fluke {
	// These hooks are called when window focus is changed.
	void on_focus_in(fluke::Connection& conn) {}
	void on_focus_out(fluke::Connection& conn) {}


	// These hooks are called when the mouse enters/leaves a window.
	void on_hover_in(fluke::Connection& conn) {}
	void on_hover_out(fluke::Connection& conn) {}


	// Called on window creation or destruction.
	void on_create(fluke::Connection& conn) {}
	void on_destroy(fluke::Connection& conn) {}


	// Called when windows are mapped or unmapped.
	void on_map(fluke::Connection& conn) {}
	void on_unmap(fluke::Connection& conn) {}


	// This is called when a window asks to be configured.
	void on_configure(fluke::Connection& conn) {}


	// This is called when a grabbed key is pressed.
	void on_keypress(fluke::Connection& conn) {}


	// Called when an error occurs.
	void on_error(fluke::Connection& conn) {}

}
