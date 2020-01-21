#pragma once


// User hooks
namespace fluke {
	// Hooks called on launch and exit.
	inline void on_launch(fluke::Connection& conn) {}
	inline void on_exit(fluke::Connection& conn) {}


	// These hooks are called when window focus is changed.
	inline void on_focus_in(fluke::Connection& conn) {}
	inline void on_focus_out(fluke::Connection& conn) {}


	// These hooks are called when the mouse enters/leaves a window.
	inline void on_hover_in(fluke::Connection& conn) {}
	inline void on_hover_out(fluke::Connection& conn) {}


	// Called on window creation or destruction.
	inline void on_create(fluke::Connection& conn) {}
	inline void on_destroy(fluke::Connection& conn) {}


	// Called when windows are mapped or unmapped.
	inline void on_map(fluke::Connection& conn) {}
	inline void on_unmap(fluke::Connection& conn) {}


	// This is called when a window asks to be configured.
	inline void on_configure(fluke::Connection& conn) {}


	// This is called when a grabbed key is pressed.
	inline void on_keypress(fluke::Connection& conn) {}


	// Called when an error occurs.
	inline void on_error(fluke::Connection& conn) {}


	// These are usually called when anything EWMH happens.
	inline void on_property(fluke::Connection& conn) {}
	inline void on_client_message(fluke::Connection& conn) {}


	// Randr/screen hooks.
	inline void on_randr_screen_change(fluke::Connection& conn) {}
	inline void on_randr_notify(fluke::Connection& conn) {}
}

