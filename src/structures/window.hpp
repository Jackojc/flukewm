#pragma once

#include <memory>
#include <util/xcb.hpp>
#include <util/exception.hpp>
#include <structures/connection.hpp>
#include <structures/attributes.hpp>
#include <structures/configure.hpp>

namespace fluke {

	struct Window {

		fluke::Connection conn;
		xcb_window_t win;


		Window(const fluke::Connection& c, xcb_window_t w)
			: conn(c), win(w)
		{
			auto cookie = xcb_get_window_attributes(conn, w);
			auto reply  = xcb_get_window_attributes_reply(conn, cookie, nullptr);

			if (not reply)
				throw fluke::except::WindowError("window '", fluke::to_hex(win), "' does not exist!");
		}


		operator xcb_window_t() const noexcept {
			return win;
		}


		fluke::Attributes attr() const {
			return fluke::Attributes{conn, win};
		}


		fluke::Configure conf() const {
			return fluke::Configure{conn, win};
		}


		void close() {
			// Just cache some atom info.
			auto atom_proto = conn.get_atom("WM_PROTOCOLS");
			auto atom_del   = conn.get_atom("WM_DELETE_WINDOW");

			auto cookie = xcb_icccm_get_wm_protocols_unchecked(conn, *this, atom_proto);


			// If window doesn't care about EWMH, just close it.
			xcb_icccm_get_wm_protocols_reply_t reply;
			if (not xcb_icccm_get_wm_protocols_reply(conn, cookie, &reply, nullptr)) {
				xcb_kill_client(conn, *this);
				return;
			}


			// Otherwise it must be EWMH compliant.
			xcb_client_message_event_t ev = { XCB_CLIENT_MESSAGE, 32, 0, *this, atom_proto, 0};

			ev.data.data32[0] = atom_del;
			ev.data.data32[1] = XCB_CURRENT_TIME;

			xcb_send_event(conn, 0, *this, XCB_EVENT_MASK_NO_EVENT, reinterpret_cast<char*>(&ev));
		}


		void kill() {
			xcb_kill_client(conn, *this);
		}


		void grow(int16_t horz, int16_t vert) {
			auto [x, y, w, h, border, mapped, ignored, root] = attr().get();

			x -= horz;
			y -= vert;
			w += horz * 2;
			h += vert * 2;

			conf().pos(x, y).size(w, h).commit();
		}


		void grow(int16_t size) {
			grow(size, size);
		}


		void shrink(int16_t horz, int16_t vert) {
			auto [x, y, w, h, border, mapped, ignored, root] = attr().get();

			x += horz;
			y += vert;
			w -= horz * 2;
			h -= vert * 2;

			conf().pos(x, y).size(w, h).commit();
		}


		void shrink(int16_t size) {
			shrink(size, size);
		}


		void move() {

		}

	};

}

