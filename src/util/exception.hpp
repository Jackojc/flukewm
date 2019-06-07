#pragma once

#include <string>
#include <stdexcept>

#define NEW_EXCEPTION_TYPE(name, default_msg)                      \
	struct name: public std::runtime_error {                       \
		std::string msg;                                           \
		template <typename... Ts> name(Ts&&... args)               \
			: std::runtime_error(((std::string{args}) + ...)),     \
			  msg(((std::string{args}) + ...)) {}                  \
		name()                                                     \
			: std::runtime_error(default_msg), msg(default_msg) {} \
	};                                                             \


namespace fluke::except {
	NEW_EXCEPTION_TYPE(ConnectionError, "could not initialise X server connection!")
	NEW_EXCEPTION_TYPE(GeometryError,   "could not get geometry!")
	NEW_EXCEPTION_TYPE(ConfigureError,  "could not configure window!")
	NEW_EXCEPTION_TYPE(AttributeError,  "could not get attributes!")
	NEW_EXCEPTION_TYPE(WindowError,     "could not get window!")
	NEW_EXCEPTION_TYPE(CloseError,      "could not close window!")
}

#undef NEW_EXCEPTION_TYPE
