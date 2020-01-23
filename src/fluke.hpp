#pragma once

// #define TINGE_PLAIN

#include <config/options.hpp>
#include <tinge.hpp>

/*
	Macro for toggling code on/off depending on whether we are in release or
	debug builds.

	example:
		FLUKE_DEBUG( std::cerr << "debug string\n" )
*/
#ifdef NDEBUG
	#define FLUKE_DEBUG(x) { }
#else
	#define FLUKE_DEBUG(x) { x; }
#endif

#define FLUKE_DEBUG_NOTICE(...)  FLUKE_DEBUG( tinge::noticeln(__VA_ARGS__) )
#define FLUKE_DEBUG_WARN(...)    FLUKE_DEBUG( tinge::warnln(__VA_ARGS__) )
#define FLUKE_DEBUG_ERROR(...)   FLUKE_DEBUG( tinge::errorln(__VA_ARGS__) )
#define FLUKE_DEBUG_SUCCESS(...) FLUKE_DEBUG( tinge::successln(__VA_ARGS__) )

// Printing functions indented with 1 tab, kind of like subheadings.
#define FLUKE_DEBUG_NOTICE_SUB(...)  FLUKE_DEBUG( tinge::noticeln(tinge::before{'\t'}, __VA_ARGS__) )
#define FLUKE_DEBUG_WARN_SUB(...)    FLUKE_DEBUG( tinge::warnln(tinge::before{'\t'}, __VA_ARGS__) )
#define FLUKE_DEBUG_ERROR_SUB(...)   FLUKE_DEBUG( tinge::errorln(tinge::before{'\t'}, __VA_ARGS__) )
#define FLUKE_DEBUG_SUCCESS_SUB(...) FLUKE_DEBUG( tinge::successln(tinge::before{'\t'}, __VA_ARGS__) )

#include <xcb/xcb.hpp>
#include <xcb/xcb_errors.hpp>

#include <structures/types.hpp>
#include <structures/connection.hpp>
#include <structures/request.hpp>

#include <utils/zip.hpp>
#include <utils/exec.hpp>
#include <utils/events.hpp>
#include <utils/keys.hpp>
#include <utils/misc.hpp>

#include <actions.hpp>

#include <config/keybindings.hpp>
#include <config/hooks.hpp>

#include <event_handlers.hpp>
