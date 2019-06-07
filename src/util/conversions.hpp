#pragma once

#include <iomanip>
#include <sstream>
#include <string>

namespace fluke {
	template <typename T>
	inline std::string to_hex(T num) {
		std::stringstream s;
		s << "0x" << std::setfill('0') << std::setw(sizeof(T) * 2) << std::hex << num;
		return s.str();
	}
}
