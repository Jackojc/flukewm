#pragma once

#include <type_traits>
#include <utility>
#include <tuple>

#include <fluke.hpp>

namespace fluke {
	template <typename... Ts>
	class RequestBuffer {
		// Data
		private:
			std::tuple<Ts...> requests;


		// Constructor
		public:
			RequestBuffer(Ts&&... args):
				requests(std::forward<Ts>(args)...) {}


		// Functions
		private:
			// Return empty tuple or tuple of forwarded argument depending on
			// tag type.
			template <typename Tag, typename T>
			constexpr static auto get_element(fluke::Connection& conn, T&& arg) {
				// If it's a getter, return tuple.
				if constexpr(std::is_same_v<Tag, detail::GetterTag>)
					return std::tuple{ arg.get(conn) };

				// If it's a setter, return empty tuple.
				if constexpr(std::is_same_v<Tag, detail::SetterTag>)
					return std::tuple{ };

				else
					throw fluke::UnknownTagError();
			}


		// Functions
		public:
			/*
				Call `.get()` on all of the tuple elements in this->requests and return
				a new tuple with the return values.

				(GetGeometry, GetWindowAttributes) -> (get_geometry_reply_t, get_window_attributes_reply_t)
			*/
			constexpr auto get(fluke::Connection& conn) const && {
				// Return a tuple of elements with the GetterTag
				auto&& ret = std::apply([&conn] (Ts... args) {
					return std::tuple_cat(get_element<typename Ts::tag_t>(conn, args)...);
				}, requests);

				// Check how many elements are in the tuple.
				constexpr auto size = std::tuple_size_v<std::decay_t<decltype(ret)>>;

				if constexpr(size > 1) {
					return std::move(ret);

				} else if constexpr(size == 1) {
					return std::move(std::get<0>(ret));
				}
			}
	};

}
