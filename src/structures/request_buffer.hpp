#pragma once

#include <type_traits>
#include <utility>
#include <tuple>
#include <util/tags.hpp>
#include <util/exception.hpp>

namespace fluke {

	template <typename... Ts>
	class RequestBuffer {
		std::tuple<Ts...> requests;

	public:
		RequestBuffer(Ts&&... args)
			: requests(std::forward<Ts>(args)...) {}


		template <typename Tag, typename T>
		constexpr static auto get_as_tuple(T&& arg) {
			// If it's a getter, return tuple.
			if constexpr(std::is_same_v<Tag, detail::GetterTag>)
				return std::tuple{ std::forward<T>(arg) };

			// If it's a setter, return empty tuple.
			if constexpr(std::is_same_v<Tag, detail::SetterTag>)
				return std::tuple{ };

			else
				throw fluke::UnknownTagError();
		}


		/*
			Call `.get()` on all of the tuple elements in this->requests and return
			a new tuple with the return values.

			(GetGeometry, GetWindowAttributes) -> (get_geometry_reply_t, get_window_attributes_reply_t)
		*/
		constexpr auto get() const {
			return std::apply([] (Ts... args) {
				return std::tuple_cat(get_as_tuple<typename Ts::tag_t>(args.get())...);
			}, requests);
		}
	};

}
