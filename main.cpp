#include <iostream>
#include <chrono>
#include <thread>

#include <tinge.hpp>
#include <fluke.hpp>


void test(fluke::Connection&) {
	tinge::successln("hello from hook1");
}


int main() {
	try {
		fluke::Connection conn;

		// register to receive window manager events. only one window manager can be active at one time.
		fluke::SetWindowAttributes{conn, conn.root(), XCB_CW_EVENT_MASK, &fluke::XCB_WINDOWMANAGER_EVENTS}.get();

		// adopt any windows which were open at the time of fluke's launch.
		fluke::adopt_orphaned_windows(conn);


		// custom user hooks
		constexpr auto hooks = fluke::make_hooks(
			// fluke::HookEntry{ 1000, &test }
		);

		constexpr auto events = fluke::make_events(
			fluke::EventEntry{ XCB_ENTER_NOTIFY,      &fluke::event_handlers::event_enter     },
			fluke::EventEntry{ XCB_LEAVE_NOTIFY,      &fluke::event_handlers::event_leave     },
			fluke::EventEntry{ XCB_FOCUS_IN,          &fluke::event_handlers::event_focus_in  },
			fluke::EventEntry{ XCB_FOCUS_OUT,         &fluke::event_handlers::event_focus_out },
			fluke::EventEntry{ XCB_CREATE_NOTIFY,     &fluke::event_handlers::event_create    },
			fluke::EventEntry{ XCB_MAP_REQUEST,       &fluke::event_handlers::event_map       },
			fluke::EventEntry{ XCB_CONFIGURE_REQUEST, &fluke::event_handlers::event_configure }
		);



		bool running = true;


		// run hooks on seperate thread
		std::thread hook_thread;

		if constexpr(hooks.size() > 0) {
			hook_thread = std::thread([&hooks, &running] () {
				fluke::Connection hook_conn;

				// find smallest delay and use that for sleep timer.
				int delay = 99999;
				for (auto&& [ms, func]: hooks) {
					if (ms < delay)
						delay = ms;
				}

				int current_ms = 0;

				while (running) {
					// handle user defined hooks.
					fluke::handle_hooks(hook_conn, hooks, current_ms);
					std::this_thread::sleep_for(std::chrono::milliseconds{delay});
					current_ms += delay;
				}
			});
		}


		conn.flush();

		// main event loop
		while (running) {
			// handle events (blocking)
			running = fluke::handle_events(conn, events);
		}


		// join hook_thread
		if constexpr(hooks.size() > 0)
			hook_thread.join();

	} catch (const fluke::SetWindowAttributesError& e) {
		tinge::noticeln(e.what());
		tinge::errorln("fluke: another window manager is already running!");
		return 1;

	} catch (const fluke::ConnectionError& e) {
		tinge::errorln("fluke: cannot connect to X!");
		return 2;

	} catch (const fluke::ScreenError& e) {
		tinge::errorln("fluke: cannot get screen information!");
		return 2;
	}

	return 1;
}
