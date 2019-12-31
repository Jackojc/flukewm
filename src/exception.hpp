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
	NEW_EXCEPTION_TYPE(ConnectionError,              "could not initialise X server connection!")
	NEW_EXCEPTION_TYPE(ScreenError,                  "could not get screen!")
	NEW_EXCEPTION_TYPE(UnknownTagError,              "unknown tag type!")

	NEW_EXCEPTION_TYPE(InternAtomError,              "could not get window atom!")
	NEW_EXCEPTION_TYPE(GetWindowAttributesError,     "could not get window attributes!")
	NEW_EXCEPTION_TYPE(GetGeometryError,             "could not get window geometry!")
	NEW_EXCEPTION_TYPE(GetPropertyError,             "could not get window property!")
	NEW_EXCEPTION_TYPE(GetInputFocusError,           "could not get input focus!")
	NEW_EXCEPTION_TYPE(QueryTreeError,               "could not query tree!")
	NEW_EXCEPTION_TYPE(QueryPointerError,            "could not query pointer!")
	NEW_EXCEPTION_TYPE(RandrGetProvidersError,       "could not get providers!")
	NEW_EXCEPTION_TYPE(RandrGetProviderInfoError,    "could not get provider info!")
	NEW_EXCEPTION_TYPE(RandrGetOutputInfoError,      "could not get output info!")
	NEW_EXCEPTION_TYPE(RandrGetCrtcInfoError,        "could not get CRTC into!")
	NEW_EXCEPTION_TYPE(RandrGetOutputPrimaryError,   "could not get primary output!")

	NEW_EXCEPTION_TYPE(ConfigureWindowError,         "could not configure window!")
	NEW_EXCEPTION_TYPE(ChangeWindowAttributesError,  "could not change window attributes!")
	NEW_EXCEPTION_TYPE(SetInputFocusError,           "could not change focus!")
	NEW_EXCEPTION_TYPE(MapWindowError,               "could not map window!")
	NEW_EXCEPTION_TYPE(UnmapWindowError,             "could not unmap window!")

}

#undef NEW_EXCEPTION_TYPE
