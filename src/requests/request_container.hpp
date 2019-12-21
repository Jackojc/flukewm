#pragma once

#include <vector>
#include <fluke.hpp>

namespace fluke {
	template <typename T>
	class RequestContainer {
		private:
			fluke::Connection conn;
			std::vector<T> elements;


		public:
			RequestContainer(const fluke::Connection& conn_):
				conn(conn_) {}

			// // Pass a series of requests to the constructor directly.
			// template <typename... Ts>
			// RequestContainer(const fluke::Connection& conn_, Ts&&... args):
			// 	conn(conn_), elements{{T{conn, std::forward<Ts>(args)}...}}
			// {

			// }

			// Pass a vector of arguments for each request.
			template <typename V, typename... Ts>
			RequestContainer(const fluke::Connection& conn_, const V& container, Ts&&... args):
				conn(conn_), elements{}
			{
				for (auto&& r: container)
					add(r, std::forward<Ts>(args)...);
			}

		public:
			auto cbegin() const { return elements.cbegin(); }
			auto cend() const { return elements.cend(); }

			auto rbegin() const { return elements.rbegin(); }
			auto rend() const { return elements.rend(); }

			auto begin() { return elements.begin(); }
			auto end() { return elements.end(); }

			[[nodiscard]] bool empty() const { return elements.empty(); }

			auto size() const { return elements.size(); }


		public:
			template <typename... Ts>
			auto add(Ts&&... args) {
				return elements.emplace_back(conn, std::forward<Ts>(args)...);
			}

			auto get() {
				std::vector<decltype(elements.begin()->get())> replies;
				replies.reserve(elements.size());

				for (auto& req: elements) {
					replies.emplace_back(req.get());
				}

				return replies;
			}
	};


	// template <typename T, typename... Ts>
	// auto create_container_from_args(fluke::Connection& conn, Ts&&... args) {
	// 	return fluke::RequestContainer<T> {
	// 		conn,
	// 		std::forward<Ts>(args)...
	// 	};
	// }


	// template <typename T, typename V>
	// auto create_container_from_vector(fluke::Connection& conn, const V& vec) {
	// 	return fluke::RequestContainer<T> {conn, vec};
	// }


	// get from multiple containers.
	template <typename... Ts>
	auto get(Ts&&... args) {
		return std::forward_as_tuple(args.get()...);
	}
}
