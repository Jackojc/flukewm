#include <iostream>
#include <tuple>
#include <vector>
#include <variant>
#include <utility>
#include <memory>
#include <string>
#include <stdexcept>
#include <cstdlib>

#include <tinge.hpp>
#include <util/xcb.hpp>

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

	#define NEW_REQUEST(name, type, msg, impl, tagtype)                                      \
		NEW_EXCEPTION_TYPE(name##Error, msg)                                                 \
		struct name {                                                                        \
			using tag = tagtype;                                                             \
			fluke::Connection conn;                                                          \
			xcb_##type##_cookie_t cookie;                                                    \
			template <typename... Ts> constexpr name(const fluke::Connection& conn_, Ts&&... args):    \
				conn(conn_),                                                                 \
				cookie(fluke::new_##type##_request(conn_, std::forward<Ts>(args)...)) {}     \
			auto get() const {                                                               \
				auto ret = fluke::type##_reply_t{xcb_##type##_reply(conn, cookie, nullptr), std::free}; \
				if (static_cast<bool>(ret) != impl)                                          \
					return ret;                                                              \
				throw fluke::name##Error();                                                  \
			}                                                                                \
		};

	#define GET_REQUEST(name, type, msg)                              \
		constexpr auto new_##type##_request = xcb_##type;             \
		using type##_reply_t = std::unique_ptr<xcb_##type##_reply_t, decltype(&std::free)>; \
		NEW_REQUEST(name, type, msg, false, GetterTag)                \

	#define SET_REQUEST(name, type, msg)                                                                        \
		using type##_reply_t = std::unique_ptr<xcb_generic_error_t, decltype(&std::free)>;                       \
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
			: conn(xcb_connect(nullptr, nullptr), [] (auto c) { xcb_disconnect(c); })
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
		std::tuple<Ts...> requests;

	public:
		RequestBuffer(Ts&&... args)
			: requests(std::forward<Ts>(args)...) {}


		template <typename Tag, typename T>
		constexpr static auto get_as_tuple(T&& arg) {
			// If it's a getter, return tuple.
			if constexpr(std::is_same_v<Tag, GetterTag>)
				return std::tuple{ std::forward<T>(arg) };

			// If it's a setter, return empty tuple.
			else
				return std::tuple{};
		}


		/*
			Call `.get()` on all of the tuple elements in this->requests and return
			a new tuple with the return values.

			(GetGeometry, GetWindowAttributes) -> (get_geometry_reply_t, get_window_attributes_reply_t)
		*/
		constexpr auto get() const {
			return std::apply([] (Ts... args) {
				return std::tuple_cat(
					get_as_tuple<typename Ts::tag>(args.get())...
				);
			}, requests);
		}
	};

}







int main(int argc, const char* argv[]) {

	if (argc < 2) {
		tinge::errorln_em("error: ", "no window specified!");
		return 1;
	}


	fluke::Connection conn;
	xcb_window_t win = std::strtoul(argv[1], nullptr, 16);


	try {
		std::vector<uint32_t> new_position = {0, 0};

		auto&& [geom, attr] = fluke::RequestBuffer{
			fluke::GetGeometry{conn, win},
			fluke::GetWindowAttributes{conn, win},
			fluke::ConfigureWindow{conn, win, XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y, new_position.data()}
		}.get();



		std::cerr << geom->x << ", " << geom->y << ", " << geom->width << ", " << geom->height << '\n';
		std::cerr << "visible? " << (attr->override_redirect ? "no" : "yes") << '\n';


	} catch (const std::exception& e) {
		tinge::errorln_em("error: ", e.what());
		return 1;
	}


	return 0;
}
