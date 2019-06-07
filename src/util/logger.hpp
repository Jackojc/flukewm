#pragma once

#include <iostream>
#include <utility>
#include <modules/rang.hpp>

// LIR_LOG_OFF:  disables all logging.

namespace fluke {
	#define NEW_COLOUR(name, val) constexpr auto name = val;

	namespace detail::colour {
		NEW_COLOUR(notice,  rang::fgB::yellow)
		NEW_COLOUR(warn,    rang::fgB::blue)
		NEW_COLOUR(error,   rang::fgB::red)
		NEW_COLOUR(success, rang::fgB::green)
	}

	namespace detail {
		struct Reset   { };
		struct DimFG   { decltype(rang::fg::black) colour; };
		struct DimBG   { decltype(rang::bg::black) colour; };
		struct Notice  { };
		struct Warn    { };
		struct Error   { };
		struct Success { };


		inline std::ostream& operator<<(std::ostream& os, const Reset&) {
			return (os << rang::style::reset << rang::fg::reset << rang::bg::reset);
		}

		inline std::ostream& operator<<(std::ostream& os, const DimFG& obj) {
			return (os << rang::style::dim << obj.colour);
		}

		inline std::ostream& operator<<(std::ostream& os, const DimBG& obj) {
			return (os << rang::style::dim << obj.colour);
		}

		inline std::ostream& operator<<(std::ostream& os, const Notice&) {
			return (os << rang::style::bold << detail::colour::notice);
		}

		inline std::ostream& operator<<(std::ostream& os, const Warn&) {
			return (os << rang::style::bold << detail::colour::warn);
		}

		inline std::ostream& operator<<(std::ostream& os, const Error&) {
			return (os << rang::style::bold << detail::colour::error);
		}

		inline std::ostream& operator<<(std::ostream& os, const Success&) {
			return (os << rang::style::bold << detail::colour::success);
		}
	}


	// Define all colours in fg::, fg::bright, fg::dim, bg::, bg::bright, bg::dim.
	namespace colour {
		namespace fg {
			NEW_COLOUR(black,   rang::fg::black)
			NEW_COLOUR(red,     rang::fg::red)
			NEW_COLOUR(green,   rang::fg::green)
			NEW_COLOUR(yellow,  rang::fg::yellow)
			NEW_COLOUR(blue,    rang::fg::blue)
			NEW_COLOUR(magenta, rang::fg::magenta)
			NEW_COLOUR(cyan,    rang::fg::cyan)
			NEW_COLOUR(grey,    rang::fg::gray)
			NEW_COLOUR(normal,  rang::fg::reset)
			NEW_COLOUR(reset,   rang::fg::reset)

			namespace bright {
				NEW_COLOUR(black,   rang::fgB::black)
				NEW_COLOUR(red,     rang::fgB::red)
				NEW_COLOUR(green,   rang::fgB::green)
				NEW_COLOUR(yellow,  rang::fgB::yellow)
				NEW_COLOUR(blue,    rang::fgB::blue)
				NEW_COLOUR(magenta, rang::fgB::magenta)
				NEW_COLOUR(cyan,    rang::fgB::cyan)
				NEW_COLOUR(grey,    rang::fgB::gray)
				NEW_COLOUR(normal,  rang::fg::reset)
				NEW_COLOUR(reset,   rang::fg::reset)
			}

			namespace dim {
				NEW_COLOUR(black,   detail::DimFG{colour::fg::black})
				NEW_COLOUR(red,     detail::DimFG{colour::fg::red})
				NEW_COLOUR(green,   detail::DimFG{colour::fg::green})
				NEW_COLOUR(yellow,  detail::DimFG{colour::fg::yellow})
				NEW_COLOUR(blue,    detail::DimFG{colour::fg::blue})
				NEW_COLOUR(magenta, detail::DimFG{colour::fg::magenta})
				NEW_COLOUR(cyan,    detail::DimFG{colour::fg::cyan})
				NEW_COLOUR(grey,    detail::DimFG{colour::fg::grey})
				NEW_COLOUR(normal,  rang::style::reset)
			}

		}

		namespace bg {
			NEW_COLOUR(black,   rang::bg::black)
			NEW_COLOUR(red,     rang::bg::red)
			NEW_COLOUR(green,   rang::bg::green)
			NEW_COLOUR(yellow,  rang::bg::yellow)
			NEW_COLOUR(blue,    rang::bg::blue)
			NEW_COLOUR(magenta, rang::bg::magenta)
			NEW_COLOUR(cyan,    rang::bg::cyan)
			NEW_COLOUR(grey,    rang::bg::gray)
			NEW_COLOUR(normal,  rang::bg::reset)
			NEW_COLOUR(reset,   rang::bg::reset)

			namespace bright {
				NEW_COLOUR(black,   rang::bgB::black)
				NEW_COLOUR(red,     rang::bgB::red)
				NEW_COLOUR(green,   rang::bgB::green)
				NEW_COLOUR(yellow,  rang::bgB::yellow)
				NEW_COLOUR(blue,    rang::bgB::blue)
				NEW_COLOUR(magenta, rang::bgB::magenta)
				NEW_COLOUR(cyan,    rang::bgB::cyan)
				NEW_COLOUR(grey,    rang::bgB::gray)
				NEW_COLOUR(normal,  rang::bg::reset)
				NEW_COLOUR(reset,   rang::bg::reset)
			}

			namespace dim {
				NEW_COLOUR(black,   detail::DimBG{colour::bg::black})
				NEW_COLOUR(red,     detail::DimBG{colour::bg::red})
				NEW_COLOUR(green,   detail::DimBG{colour::bg::green})
				NEW_COLOUR(yellow,  detail::DimBG{colour::bg::yellow})
				NEW_COLOUR(blue,    detail::DimBG{colour::bg::blue})
				NEW_COLOUR(magenta, detail::DimBG{colour::bg::magenta})
				NEW_COLOUR(cyan,    detail::DimBG{colour::bg::cyan})
				NEW_COLOUR(grey,    detail::DimBG{colour::bg::grey})
				NEW_COLOUR(normal,  rang::style::reset)
			}
		}
	}

	#undef NEW_COLOUR



	// Styles
	namespace style {
		constexpr auto bold    = rang::style::bold;
		constexpr auto reverse = rang::style::reversed;

		constexpr detail::Reset   reset;
		constexpr detail::Notice  notice;
		constexpr detail::Warn    warn;
		constexpr detail::Error   error;
		constexpr detail::Success success;
	}



	// Symbols.
	namespace detail::symbol {
		constexpr auto notice  = "[ ] ";
		constexpr auto warn    = "[*] ";
		constexpr auto error   = "[!] ";
		constexpr auto success = "[^] ";
	}



	// Logging and IO.
	template <typename... Ts>
	inline std::ostream& print(Ts&&... args) {
		#ifndef LIR_LOG_OFF
			return ((std::cout << style::reset) << ... << std::forward<Ts&&>(args)) << style::reset;
		#else
			return std::cout;
		#endif
	}



	// Print.
	#define OUT(name)                                                                                         \
		template <typename... Ts>                                                                             \
		inline std::ostream& name(Ts&&... args) {                                                             \
			return fluke::print(style::name, detail::symbol::name, style::reset, std::forward<Ts&&>(args)...); \
		}

	OUT(notice)
	OUT(warn)
	OUT(success)
	OUT(error)

	#undef OUT



	// Print with linebreak.
	#define OUTLN(name)                                           \
		template <typename... Ts>                                 \
		inline std::ostream& name##ln(Ts&&... args) {             \
			return fluke::name(std::forward<Ts&&>(args)..., '\n'); \
		}

	OUTLN(print)
	OUTLN(notice)
	OUTLN(warn)
	OUTLN(success)
	OUTLN(error)

	#undef OUTLN



	// Emphasis.
	#define OUTEM(name)                                                                      \
		template <typename T, typename... Ts>                                                \
		inline std::ostream& name##_em(T&& first, Ts&&... args) {                            \
			return fluke::name(style::bold, first, style::reset, std::forward<Ts&&>(args)...); \
		}

	OUTEM(print)
	OUTEM(notice)
	OUTEM(warn)
	OUTEM(success)
	OUTEM(error)

	#undef OUTEM



	// Emphasis with linebreak.
	#define OUTEMLN(name)                                            \
		template <typename... Ts>                                    \
		inline std::ostream& name##ln_em(Ts&&... args) {             \
			return fluke::name##_em(std::forward<Ts&&>(args)..., '\n'); \
		}

	OUTEMLN(print)
	OUTEMLN(notice)
	OUTEMLN(warn)
	OUTEMLN(success)
	OUTEMLN(error)

	#undef OUTEMLN



	// Headings.
	#define OUTH(name)                                                  \
		template <typename... Ts>                                       \
		inline std::ostream& name##_h(Ts&&... args) {                   \
			return fluke::name(style::bold, std::forward<Ts&&>(args)...); \
		}

	OUTH(print)
	OUTH(notice)
	OUTH(warn)
	OUTH(success)
	OUTH(error)

	#undef OUTH



	// Headings with linebreak.
	#define OUTHLN(name)                                             \
		template <typename... Ts>                                    \
		inline std::ostream& name##ln_h(Ts&&... args) {              \
			return fluke::name##_h(std::forward<Ts&&>(args)..., '\n'); \
		}

	OUTHLN(print)
	OUTHLN(notice)
	OUTHLN(warn)
	OUTHLN(success)
	OUTHLN(error)

	#undef OUTHLN
}
