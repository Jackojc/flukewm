#pragma once

#include <fluke.hpp>

namespace fluke {

	using HookCallback = void(*)(fluke::Connection&);
	using HookEntry = std::pair<int, HookCallback>;
	template <size_t N> using HookArray = std::array<HookEntry, N>;


	template <typename... Ts>
	constexpr auto make_hooks(Ts&&... args) {
		return HookArray<sizeof...(Ts)>{ std::forward<Ts>(args)... };
	}


	template <size_t N>
	void handle_hooks(fluke::Connection& conn, const HookArray<N>& hooks, int current_ms) {
		for (auto&& [ms, func]: hooks) {
			if (current_ms % ms == 0) {
				func(conn);
			}
		}
	}

}
