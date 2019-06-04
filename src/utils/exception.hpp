#pragma once

#include <string>
#include <stdexcept>

#define NEW_EXCEPTION_TYPE(name, default_msg)                      \
	struct name: public std::runtime_error {                       \
		std::string msg;                                           \
		name(const std::string& m)                                 \
			: std::runtime_error(m), msg(m) {}                     \
		name()                                                     \
			: std::runtime_error(default_msg), msg(default_msg) {} \
	};                                                             \

namespace fluke::except {
	NEW_EXCEPTION_TYPE(Generic, "error!")
}

#undef NEW_EXCEPTION_TYPE
