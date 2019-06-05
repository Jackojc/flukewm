#pragma once

extern "C" {
	#include <xcb/randr.h>
}

#include <vector>
#include <string>
#include <utils/core.hpp>

namespace fluke {
	auto get_providers(xcb_connection_t* conn, xcb_screen_t* scrn) {
		using vec_t = std::vector<xcb_randr_provider_t>;

		auto pc = xcb_randr_get_providers(conn, scrn->root);
		auto pr = xcb_randr_get_providers_reply(conn, pc, nullptr);

		if (not pr)
			fluke::error("could not acquire the array of providers!");


		vec_t vec;

		auto size = static_cast<vec_t::size_type>(xcb_randr_get_providers_providers_length(pr));
		vec.resize(size);

		auto ptr = xcb_randr_get_providers_providers(pr);
		std::copy(ptr, ptr + size, vec.begin());


		std::free(pr);

		return vec;
	}



	auto get_outputs(xcb_connection_t* conn, xcb_randr_provider_t provider) {
		using vec_t = std::vector<xcb_randr_output_t>;

		auto pic = xcb_randr_get_provider_info(conn, provider, 0);
		auto pir = xcb_randr_get_provider_info_reply(conn, pic, nullptr);

		if (not pir)
			fluke::error("could not aquire the array of outputs.");


		vec_t vec;

		auto size = static_cast<vec_t::size_type>(xcb_randr_get_provider_info_outputs_length(pir));
		vec.resize(size);

		auto ptr = xcb_randr_get_provider_info_outputs(pir);
		std::copy(ptr, ptr + size, vec.begin());

		return vec;
	}



	auto get_output_info(xcb_connection_t* conn, xcb_randr_output_t output) {
		auto oic               = xcb_randr_get_output_info(conn, output, 0);
		auto output_info_reply = xcb_randr_get_output_info_reply(conn, oic, nullptr);

		if (not output_info_reply)
			fluke::error("could not get output info.");

		return output_info_reply;
	}



	std::string get_output_name(xcb_connection_t* conn, xcb_randr_output_t output) {
		auto r = get_output_info(conn, output);

		auto name = xcb_randr_get_output_info_name(r);
		auto size = xcb_randr_get_output_info_name_length(r);

		return {reinterpret_cast<char*>(name), static_cast<std::string::size_type>(size)};
	}



	int get_output_connection(xcb_connection_t* conn, xcb_randr_output_t output) {
		return get_output_info(conn, output)->connection;
	}



	auto get_output_crtc_info(xcb_connection_t* conn, xcb_randr_crtc_t crtc) {
		auto cic = xcb_randr_get_crtc_info(conn, crtc, 0);

		return xcb_randr_get_crtc_info_reply(conn, cic, nullptr);
	}



	xcb_window_t get_focused_window(xcb_connection_t* conn) {
		auto input_focus_cookie = xcb_get_input_focus(conn);
		auto input_focus_reply  = xcb_get_input_focus_reply(conn, input_focus_cookie, nullptr);

		if (not input_focus_reply)
			fluke::error("cannot get focused window.");

		auto win = input_focus_reply->focus;

		std::free(input_focus_reply);

		return win;
	}



	auto get_window_geometry(xcb_connection_t* conn, xcb_window_t window) {
		auto get_geometry_cookie = xcb_get_geometry(conn, window);
		auto get_geometry_reply  = xcb_get_geometry_reply(conn, get_geometry_cookie, nullptr);

		if (not get_geometry_reply)
			fluke::error("no such window", window);

		return get_geometry_reply;
	}



	auto get_primary_output(xcb_connection_t* conn) {
		auto c = xcb_randr_get_output_primary(conn, get_screen(conn)->root);

		return xcb_randr_get_output_primary_reply(conn, c, nullptr);
	}
}
