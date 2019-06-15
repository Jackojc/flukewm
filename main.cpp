#include <iostream>
#include <tuple>
#include <vector>
#include <variant>
#include <utility>
#include <memory>
#include <string>
#include <stdexcept>

#include <util/xcb.hpp>
#include <util/logger.hpp>

namespace fluke {
	// hack
	#define NEW_EXCEPTION_TYPE(name, default_msg)                    \
		struct name: public std::runtime_error {                     \
			template <typename... Ts> name(Ts&&... args)             \
				: std::runtime_error(((std::string{args}) + ...)) {} \
			name()                                                   \
				: std::runtime_error(default_msg) {}                 \
		};                                                           \

	struct GetterTag {};
	struct SetterTag {};

	#define NEW_REQUEST(name, type, msg, impl, tagtype)                                          \
		NEW_EXCEPTION_TYPE(name##Error, msg)                                                     \
		struct name {                                                                            \
			using tag = tagtype;                                                                 \
			fluke::Connection conn;                                                              \
			xcb_##type##_cookie_t cookie;                                                        \
			auto get() const {                                                                   \
				if (auto ret = fluke::type##_reply_t{xcb_##type##_reply(conn, cookie, nullptr)}; \
				    static_cast<bool>(ret) != impl)                                              \
					return ret;                                                                  \
				throw fluke::name##Error();                                                      \
			}                                                                                    \
			template <typename... Ts>                                                            \
			name(const fluke::Connection& conn_, Ts&&... args):                                  \
				conn(conn_),                                                                     \
				cookie(fluke::new_##type##_request(conn_, std::forward<Ts>(args)...)) {}         \
		};                                                                                       \

	#define GET_REQUEST(name, type, msg)                              \
		constexpr auto new_##type##_request = xcb_##type;             \
		using type##_reply_t = std::unique_ptr<xcb_##type##_reply_t>; \
		NEW_REQUEST(name, type, msg, false, GetterTag)                \

	#define SET_REQUEST(name, type, msg)                                                                        \
		using type##_reply_t = std::unique_ptr<xcb_generic_error_t>;                                            \
		using xcb_##type##_cookie_t = xcb_void_cookie_t;                                                        \
		constexpr auto new_##type##_request = xcb_##type##_checked;                                             \
		template <typename C, typename P> auto xcb_##type##_reply(const fluke::Connection& conn, C cookie, P) { \
			return xcb_request_check(conn, cookie);                                                             \
		}                                                                                                       \
		NEW_REQUEST(name, type, msg, true, SetterTag)                                                           \






	NEW_EXCEPTION_TYPE(ConnectionError, "could not initialise X server connection!")

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






	// Getters
	GET_REQUEST(InternAtom,            intern_atom,               "could not get window atom!")
	GET_REQUEST(GetWindowAttributes,   get_window_attributes,     "could not get window attributes!")
	GET_REQUEST(GetGeometry,           get_geometry,              "could not get window geometry!")
	GET_REQUEST(GetProperty,           get_property,              "could not get window property!")
	GET_REQUEST(GetInputFocus,         get_input_focus,           "could not get input focus!")
	GET_REQUEST(QueryTree,             query_tree,                "could not query tree!")
	GET_REQUEST(QueryPointer,          query_pointer,             "could not query pointer!")
	GET_REQUEST(RandrGetProviders,     randr_get_providers,       "could not get providers!")
	GET_REQUEST(RandrGetProviderInfo,  randr_get_provider_info,   "could not get provider info!")
	GET_REQUEST(RandrGetOutputInfo,    randr_get_output_info,     "could not get output info!")
	GET_REQUEST(RandrGetCrtcInfo,      randr_get_crtc_info,       "could not get crtc info!")
	GET_REQUEST(RandrGetOutputPrimary, randr_get_output_primary,  "could not get primary output!")

	// Setters
	SET_REQUEST(ConfigureWindow,        configure_window,         "could not configure window!")
	SET_REQUEST(ChangeWindowAttributes, change_window_attributes, "could not change window attributes!")




	#undef NEW_REQUEST
	#undef SET_REQUEST
	#undef GET_REQUEST
	#undef NEW_EXCEPTION_TYPE





	template <typename... Ts>
	class RequestBuffer {
		using container_t  = std::tuple<Ts...>;
		container_t requests;

	public:
		RequestBuffer(Ts&&... args)
			: requests(std::forward<Ts>(args)...) {}

		/*
			Call `.get()` on all of the tuple elements in this->requests and return
			a new tuple with the return values.

			(GetGeometry, GetWindowAttributes) -> (get_geometry_reply_t, get_window_attributes_reply_t)
		*/

		// template <typename... Xs>
		// auto get_impl(auto&& x) const {

		// }

		auto get() const {
			return std::apply([] (auto&&... args) {
				return std::tuple([] (auto&& x) {
					return x.get();
				}(args)...);
			}, requests);
		}
	};
}








int main(int argc, const char* argv[]) {

	if (argc < 2) {
		fluke::errorln_em("error: ", "no window specified!");
		return 1;
	}


	fluke::Connection conn;
	xcb_window_t win = std::strtoul(argv[1], nullptr, 16);


	try {
		std::vector<uint32_t> new_position = {0, 0};

		auto requests = fluke::RequestBuffer{
			fluke::GetGeometry{conn, win},
			fluke::GetWindowAttributes{conn, win},
			fluke::ConfigureWindow{conn, win, XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y, new_position.data()}
		}.get();


		auto&& [geom, attr, _] = requests;


		std::cerr << geom->x << ", " << geom->y << ", " << geom->width << ", " << geom->height << '\n';
		std::cerr << "visible? " << (attr->override_redirect ? "no" : "yes") << '\n';


	} catch (const std::exception& e) {
		fluke::errorln_em("error: ", e.what());
		return 1;
	}


	return 0;
}
