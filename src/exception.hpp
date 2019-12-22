#pragma once

#include <string>
#include <stdexcept>

#define NEW_EXCEPTION_TYPE(name, default_msg)                    \
	struct name: public std::runtime_error {                     \
		template <typename... Ts> name(Ts&&... args)             \
			: std::runtime_error(((std::string{args}) + ...)) {} \
		name()                                                   \
			: std::runtime_error(default_msg) {}                 \
	};                                                           \

namespace fluke {
	NEW_EXCEPTION_TYPE(ConnectionError, "could not initialise X server connection!")
	NEW_EXCEPTION_TYPE(ScreenError,     "could not get screen!")
	NEW_EXCEPTION_TYPE(UnknownTagError, "unknown tag type!")

	NEW_EXCEPTION_TYPE(GetInternAtomError,         "could not get window atom!")
	NEW_EXCEPTION_TYPE(GetWindowAttributesError,   "could not get window attributes!")
	NEW_EXCEPTION_TYPE(GetWindowGeometryError,     "could not get window geometry!")
	NEW_EXCEPTION_TYPE(GetWindowPropertyError,     "could not get window property!")
	NEW_EXCEPTION_TYPE(GetInputFocusError,         "could not get input focus!")
	NEW_EXCEPTION_TYPE(GetTreeError,               "could not query tree!")
	NEW_EXCEPTION_TYPE(GetPointerError,            "could not query pointer!")
	NEW_EXCEPTION_TYPE(GetRandrProvidersError,     "could not get providers!")
	NEW_EXCEPTION_TYPE(GetRandrProviderInfoError,  "could not get provider info!")
	NEW_EXCEPTION_TYPE(GetRandrOutputInfoError,    "could not get output info!")
	NEW_EXCEPTION_TYPE(GetRandrCrtcInfoError,      "could not get CRTC into!")
	NEW_EXCEPTION_TYPE(GetRandrOutputPrimaryError, "could not get primary output!")

	NEW_EXCEPTION_TYPE(SetWindowConfigError,     "could not configure window!")
	NEW_EXCEPTION_TYPE(SetWindowAttributesError, "could not change window attributes!")
	NEW_EXCEPTION_TYPE(SetInputFocusError,       "could not change focus!")
	NEW_EXCEPTION_TYPE(SetWindowMappedError,     "could not map window!")
	NEW_EXCEPTION_TYPE(SetWindowUnmappedError,   "could not unmap window!")

}

#undef NEW_EXCEPTION_TYPE
