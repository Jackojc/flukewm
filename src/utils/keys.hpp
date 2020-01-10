#pragma once

#include <array>
#include <vector>
#include <fluke.hpp>

namespace fluke {

	using KeyCallback = void(*)(fluke::Connection&, const std::vector<int>&);

	struct Key {
		unsigned mod;
		xcb_keysym_t keysym;
		fluke::KeyCallback func;
		std::vector<int> arg;
	};


	// Basically an array with a known T.
	template <size_t N>
	struct Keys: std::array<fluke::Key, N> {};

	// Deduction guide so we can automatically determine
	// the size of the array.
	template <class... Ts>
	Keys(Ts...) -> Keys<sizeof...(Ts)>;
}
