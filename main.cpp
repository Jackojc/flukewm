#include <iostream>
#include <vector>
#include <variant>
#include <utility>
#include <memory>
#include <unordered_map>
#include <bitset>
#include <string>
#include <stdexcept>


#include <util/xcb.hpp>
#include <util/logger.hpp>



#define NEW_EXCEPTION_TYPE(name, default_msg)                    \
	struct name: public std::runtime_error {                     \
		template <typename... Ts> name(Ts&&... args)             \
			: std::runtime_error(((std::string{args}) + ...)) {} \
		name()                                                   \
			: std::runtime_error(default_msg) {}                         \
	};


namespace fluke {
	NEW_EXCEPTION_TYPE(ConnectionError,       "could not initialise X server connection!")
	NEW_EXCEPTION_TYPE(GeometryError,         "could not get geometry!")
	NEW_EXCEPTION_TYPE(ConfigureError,        "could not configure window!")
	NEW_EXCEPTION_TYPE(AttributeError,        "could not get attributes!")
	NEW_EXCEPTION_TYPE(WindowError,           "could not get window!")
	NEW_EXCEPTION_TYPE(CloseError,            "could not close window!")
	NEW_EXCEPTION_TYPE(AtomError,             "could not get atom!")
	NEW_EXCEPTION_TYPE(PropertyError,         "could not get atom!")
	NEW_EXCEPTION_TYPE(RequestError,          "could not fulfill request!")
	NEW_EXCEPTION_TYPE(InputFocusError,       "could not get input focus!")
	NEW_EXCEPTION_TYPE(QueryTreeError,        "could not query tree!")
	NEW_EXCEPTION_TYPE(QueryPointerError,     "could not query pointer!")
	NEW_EXCEPTION_TYPE(GetProvidersError,     "could not get providers!")
	NEW_EXCEPTION_TYPE(GetProviderInfoError,  "could not get provider info!")
	NEW_EXCEPTION_TYPE(GetOutputInfoError,    "could not get output info!")
	NEW_EXCEPTION_TYPE(GetCrtcInfoError,      "could not get crtc info!")
	NEW_EXCEPTION_TYPE(GetOutputPrimaryError, "could not get primary output!")
}

#undef NEW_EXCEPTION_TYPE



namespace fluke {
	struct Connection {
		std::shared_ptr<xcb_connection_t> conn;

		Connection()
			: conn(xcb_connect(nullptr, nullptr), [] (auto c) { std::free(c); })
		{
			if (xcb_connection_has_error(conn.get()))
				throw fluke::ConnectionError();
		}

		operator xcb_connection_t* () const {
			return conn.get();
		}

		// Flush all pending requests.
		void flush() noexcept {
			xcb_flush(conn.get());
		}

		// Flush all pending requests _and_ wait for them to complete.
		void sync() noexcept {
			xcb_aux_sync(conn.get());
		}
	};




	struct Request {
		xcb_window_t id;

		std::variant<
			xcb_void_cookie_t,
			xcb_intern_atom_cookie_t,

			xcb_get_window_attributes_cookie_t,
			xcb_get_geometry_cookie_t,
			xcb_get_property_cookie_t,
			xcb_get_input_focus_cookie_t,

			xcb_query_tree_cookie_t,
			xcb_query_pointer_cookie_t,

			xcb_randr_get_providers_cookie_t,
			xcb_randr_get_provider_info_cookie_t,
			xcb_randr_get_output_info_cookie_t,
			xcb_randr_get_crtc_info_cookie_t,
			xcb_randr_get_output_primary_cookie_t

		> cookie;

		template <typename T>
		Request(xcb_window_t w, T&& c)
			: id(w), cookie(c)
		{

		}
	};


	using intern_atom_reply              = std::unique_ptr<xcb_intern_atom_reply_t>;
	using get_window_attributes_reply    = std::unique_ptr<xcb_get_window_attributes_reply_t>;
	using get_geometry_reply             = std::unique_ptr<xcb_get_geometry_reply_t>;
	using get_property_reply             = std::unique_ptr<xcb_get_property_reply_t>;
	using get_input_focus_reply          = std::unique_ptr<xcb_get_input_focus_reply_t>;
	using query_tree_reply               = std::unique_ptr<xcb_query_tree_reply_t>;
	using query_pointer_reply            = std::unique_ptr<xcb_query_pointer_reply_t>;
	using randr_get_providers_reply      = std::unique_ptr<xcb_randr_get_providers_reply_t>;
	using randr_get_provider_info_reply  = std::unique_ptr<xcb_randr_get_provider_info_reply_t>;
	using randr_get_output_info_reply    = std::unique_ptr<xcb_randr_get_output_info_reply_t>;
	using randr_get_crtc_info_reply      = std::unique_ptr<xcb_randr_get_crtc_info_reply_t>;
	using randr_get_output_primary_reply = std::unique_ptr<xcb_randr_get_output_primary_reply_t>;


	using Return = std::variant<
		std::monostate,
		fluke::intern_atom_reply,
		fluke::get_window_attributes_reply,
		fluke::get_geometry_reply,
		fluke::get_property_reply,
		fluke::get_input_focus_reply,
		fluke::query_tree_reply,
		fluke::query_pointer_reply,
		fluke::randr_get_providers_reply,
		fluke::randr_get_provider_info_reply,
		fluke::randr_get_output_info_reply,
		fluke::randr_get_crtc_info_reply,
		fluke::randr_get_output_primary_reply
	>;




	struct Reply {
		fluke::Connection conn;


		fluke::Return operator()(xcb_void_cookie_t c) {
			if (std::unique_ptr<xcb_generic_error_t>(xcb_request_check(conn, c)))
				throw fluke::RequestError();
			else
				return {};
		}


		fluke::Return operator()(xcb_intern_atom_cookie_t c) {
			if (auto r = fluke::intern_atom_reply{xcb_intern_atom_reply(conn, c, nullptr)}; not r)
				throw fluke::AtomError();
			else
				return r;
		}


		fluke::Return operator()(xcb_get_window_attributes_cookie_t c) {
			if (auto r = fluke::get_window_attributes_reply{xcb_get_window_attributes_reply(conn, c, nullptr)}; not r)
				throw fluke::AttributeError();
			else
				return r;
		}


		fluke::Return operator()(xcb_get_geometry_cookie_t c) {
			if (auto r = fluke::get_geometry_reply{xcb_get_geometry_reply(conn, c, nullptr)}; not r)
				throw fluke::GeometryError();
			else
				return r;
		}


		fluke::Return operator()(xcb_get_property_cookie_t c) {
			if (auto r = fluke::get_property_reply{xcb_get_property_reply(conn, c, nullptr)}; not r)
				throw fluke::PropertyError();
			else
				return r;
		}


		fluke::Return operator()(xcb_get_input_focus_cookie_t c) {
			if (auto r = fluke::get_input_focus_reply{xcb_get_input_focus_reply(conn, c, nullptr)}; not r)
				throw fluke::InputFocusError();
			else
				return r;
		}


		fluke::Return operator()(xcb_query_tree_cookie_t c) {
			if (auto r = fluke::query_tree_reply{xcb_query_tree_reply(conn, c, nullptr)}; not r)
				throw fluke::QueryTreeError();
			else
				return r;
		}


		fluke::Return operator()(xcb_query_pointer_cookie_t c) {
			if (auto r = fluke::query_pointer_reply{xcb_query_pointer_reply(conn, c, nullptr)}; not r)
				throw fluke::QueryPointerError();
			else
				return r;
		}


		fluke::Return operator()(xcb_randr_get_providers_cookie_t c) {
			if (auto r = fluke::randr_get_providers_reply{xcb_randr_get_providers_reply(conn, c, nullptr)}; not r)
				throw fluke::GetProvidersError();
			else
				return r;
		}


		fluke::Return operator()(xcb_randr_get_provider_info_cookie_t c) {
			if (auto r = fluke::randr_get_provider_info_reply{xcb_randr_get_provider_info_reply(conn, c, nullptr)}; not r)
				throw fluke::GetProviderInfoError();
			else
				return r;
		}


		fluke::Return operator()(xcb_randr_get_output_info_cookie_t c) {
			if (auto r = fluke::randr_get_output_info_reply{xcb_randr_get_output_info_reply(conn, c, nullptr)}; not r)
				throw fluke::GetOutputInfoError();
			else
				return r;
		}


		fluke::Return operator()(xcb_randr_get_crtc_info_cookie_t c) {
			if (auto r = fluke::randr_get_crtc_info_reply{xcb_randr_get_crtc_info_reply(conn, c, nullptr)}; not r)
				throw fluke::GetCrtcInfoError();
			else
				return r;
		}


		fluke::Return operator()(xcb_randr_get_output_primary_cookie_t c) {
			if (auto r = fluke::randr_get_output_primary_reply{xcb_randr_get_output_primary_reply(conn, c, nullptr)}; not r)
				throw fluke::GetOutputPrimaryError();
			else
				return r;
		}
	};




	enum {
		MASK_X,
		MASK_Y,
		MASK_WIDTH,
		MASK_HEIGHT,
		MASK_STACK,
		MASK_MAPPED,
		MASK_VISIBLE,
		MASK_BORDER_COLOUR,
		MASK_BORDER_WIDTH,
		MASK_TOTAL
	};



	struct Windows {
		template <typename T> using wmap = std::unordered_map<xcb_window_t, T>;

		wmap<std::bitset<MASK_TOTAL>> mask;

		wmap<int16_t>  x, y;
		wmap<uint16_t> w, h;
		wmap<uint32_t> stack;

		wmap<bool> mapped;
		wmap<bool> visible;

		struct {
			wmap<uint32_t> colour;
			wmap<uint16_t> width;
		} border;
	};
}







void run(int argc, const char** argv) {
	fluke::Connection conn;
	fluke::Windows windows;

	std::vector<fluke::Request> requests;

	xcb_window_t w = std::strtoul("0x05000006", nullptr, 16);
	requests.emplace_back(w, xcb_get_window_attributes(conn, w));
	requests.emplace_back(w, xcb_get_geometry(conn, w));


	// add support configuring window using these requests, add a struct of values to the variant that can be used with configure.


	for (const auto& x: requests) {
		auto& [win, cookie] = x;
		auto reply = std::visit(fluke::Reply{conn}, cookie);

		std::visit([win, &windows] (auto&& r) {
			using T = std::decay_t<decltype(r)>;


			if constexpr (std::is_same_v<T, fluke::intern_atom_reply>) {
				std::cerr << "xcb_query_pointer_reply_t\n";

			} else if constexpr (std::is_same_v<T, fluke::get_window_attributes_reply>) {
				std::cerr << "xcb_get_window_attributes_reply_t\n";
				windows.mapped[win] = r->map_state == XCB_MAP_STATE_VIEWABLE;
				windows.visible[win] = not r->override_redirect;
				windows.mask[win][fluke::MASK_MAPPED] = true;
				windows.mask[win][fluke::MASK_VISIBLE] = true;

			} else if constexpr (std::is_same_v<T, fluke::get_geometry_reply>) {
				std::cerr << "xcb_get_geometry_reply_t\n";
				windows.x[win] = r->x;
				windows.y[win] = r->y;
				windows.w[win] = r->width;
				windows.h[win] = r->height;
				windows.mask[win][fluke::MASK_X] = true;
				windows.mask[win][fluke::MASK_Y] = true;
				windows.mask[win][fluke::MASK_WIDTH] = true;
				windows.mask[win][fluke::MASK_HEIGHT] = true;

			} else if constexpr (std::is_same_v<T, fluke::get_property_reply>) {
				std::cerr << "xcb_get_property_reply_t\n";

			} else if constexpr (std::is_same_v<T, fluke::get_input_focus_reply>) {
				std::cerr << "xcb_get_input_focus_reply_t\n";

			} else if constexpr (std::is_same_v<T, fluke::query_tree_reply>) {
				std::cerr << "xcb_query_tree_reply_t\n";

			} else if constexpr (std::is_same_v<T, fluke::query_pointer_reply>) {
				std::cerr << "xcb_query_pointer_reply_t\n";

			} else if constexpr (std::is_same_v<T, fluke::randr_get_providers_reply>) {
				std::cerr << "xcb_randr_get_providers_reply_t\n";

			} else if constexpr (std::is_same_v<T, fluke::randr_get_provider_info_reply>) {
				std::cerr << "xcb_randr_get_provider_info_reply_t\n";

			} else if constexpr (std::is_same_v<T, fluke::randr_get_output_info_reply>) {
				std::cerr << "xcb_randr_get_output_info_reply_t\n";

			} else if constexpr (std::is_same_v<T, fluke::randr_get_crtc_info_reply>) {
				std::cerr << "xcb_randr_get_crtc_info_reply_t\n";

			} else if constexpr (std::is_same_v<T, fluke::randr_get_output_primary_reply>) {
				std::cerr << "xcb_randr_get_output_primary_reply_t\n";

			} else {
				std::cerr << "xcb_void_cookie_t\n";
			}

		}, reply);
	}
}






int main(int argc, const char* argv[]) {
	try {
		run(argc, argv);

	} catch (const std::exception& e) {
		fluke::errorln_em("error: ", e.what());
		return 1;
	}

	return 0;
}
