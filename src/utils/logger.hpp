#pragma once


#include <cstdint>
#include <string>
#include <iostream>
#include <vector>
#include <array>
#include <utility>
#include <modules/rang.hpp>


/*
	Macros and constants:

	LIR_LOG_OFF:  disables all logging.
	LIR_PIPE_OFF: disables all piping.
*/



namespace fluke {
	// Macros
	#define NEW_COLOUR(name, val) constexpr auto name = val;


	// Colours.
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
			}
		}


		NEW_COLOUR(notice,  fg::bright::yellow)
		NEW_COLOUR(warn,    fg::bright::blue)
		NEW_COLOUR(error,   fg::bright::red)
		NEW_COLOUR(success, fg::bright::green)
	}






	// Styles.
	namespace style {
		// These are the most universally supported styles.
		constexpr auto bold = rang::style::bold;
		constexpr auto reverse = rang::style::reversed;
	}



	namespace details {
		struct Reset {};
		struct DimFG { decltype(colour::fg::black) colour; };
		struct DimBG { decltype(colour::bg::black) colour; };
		struct Notice {};
		struct Warn {};
		struct Error {};
		struct Success {};


		// Overloads.
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
			return (os << style::bold << colour::notice);
		}


		inline std::ostream& operator<<(std::ostream& os, const Warn&) {
			return (os << style::bold << colour::warn);
		}


		inline std::ostream& operator<<(std::ostream& os, const Error&) {
			return (os << style::bold << colour::error);
		}


		inline std::ostream& operator<<(std::ostream& os, const Success&) {
			return (os << style::bold << colour::success);
		}
	}



	namespace colour {
		namespace fg::dim {
			NEW_COLOUR(black,   details::DimFG{colour::fg::black})
			NEW_COLOUR(red,     details::DimFG{colour::fg::red})
			NEW_COLOUR(green,   details::DimFG{colour::fg::green})
			NEW_COLOUR(yellow,  details::DimFG{colour::fg::yellow})
			NEW_COLOUR(blue,    details::DimFG{colour::fg::blue})
			NEW_COLOUR(magenta, details::DimFG{colour::fg::magenta})
			NEW_COLOUR(cyan,    details::DimFG{colour::fg::cyan})
			NEW_COLOUR(grey,    details::DimFG{colour::fg::grey})
			NEW_COLOUR(normal,  rang::style::reset)
		}

		namespace bg::dim {
			NEW_COLOUR(black,   details::DimBG{colour::bg::black})
			NEW_COLOUR(red,     details::DimBG{colour::bg::red})
			NEW_COLOUR(green,   details::DimBG{colour::bg::green})
			NEW_COLOUR(yellow,  details::DimBG{colour::bg::yellow})
			NEW_COLOUR(blue,    details::DimBG{colour::bg::blue})
			NEW_COLOUR(magenta, details::DimBG{colour::bg::magenta})
			NEW_COLOUR(cyan,    details::DimBG{colour::bg::cyan})
			NEW_COLOUR(grey,    details::DimBG{colour::bg::grey})
			NEW_COLOUR(normal,  rang::style::reset)
		}
	}



	// Remove all macros.
	#undef NEW_COLOUR





	// Custom styles
	namespace style {
		inline details::Reset   reset;
		inline details::Notice  notice;
		inline details::Warn    warn;
		inline details::Error   error;
		inline details::Success success;
	}



	// Output symbols
	namespace details {
		namespace symbol {
			constexpr auto notice  = "[ ] ";
			constexpr auto warn    = "[*] ";
			constexpr auto error   = "[!] ";
			constexpr auto success = "[^] ";
		}
	}



	// Streams.
	inline std::ostream& piper = std::cout;
	inline std::ostream& printer = std::cerr;




	// Logging and IO.
	template <typename... Ts>
	inline std::ostream& print(Ts&&... args) {
		#ifndef LIR_LOG_OFF
			return ((fluke::printer << style::reset) << ... << std::forward<Ts&&>(args)) << style::reset;
		#else
			return fluke::printer;
		#endif
	}



	template <typename... Ts>
	inline std::ostream& pipe(Ts&&... args) {
		#ifndef LIR_PIPE_OFF
			return (fluke::piper << ... << std::forward<Ts&&>(args));
		#else
			return fluke::piper;
		#endif
	}






	// Logging and IO...
	template <typename... Ts>
	inline std::ostream& println(Ts&&... args) {
		#ifndef LIR_LOG_OFF
			return fluke::print(std::forward<Ts&&>(args)...) << '\n';
		#else
			return fluke::printer;
		#endif
	}



	template <typename... Ts>
	inline std::ostream& pipeln(Ts&&... args) {
		#ifndef LIR_PIPE_OFF
			return fluke::pipe(std::forward<Ts&&>(args)...) << '\n';
		#else
			return fluke::piper;
		#endif
	}




	// Logging with colour.
	template <typename... Ts>
	inline std::ostream& notice(Ts&&... args) {
		fluke::print(style::notice, details::symbol::notice);
		return fluke::print(std::forward<Ts&&>(args)...);
	}


	template <typename... Ts>
	inline std::ostream& warn(Ts&&... args) {
		fluke::print(style::warn, details::symbol::warn);
		return fluke::print(std::forward<Ts&&>(args)...);
	}


	template <typename... Ts>
	inline std::ostream& error(Ts&&... args) {
		fluke::print(style::error, details::symbol::error);
		return fluke::print(std::forward<Ts&&>(args)...);
	}


	template <typename... Ts>
	inline std::ostream& success(Ts&&... args) {
		fluke::print(style::success, details::symbol::success);
		return fluke::print(std::forward<Ts&&>(args)...);
	}


	// Print lines too.
	template <typename... Ts>
	inline std::ostream& noticeln(Ts&&... args) {
		return fluke::notice(std::forward<Ts&&>(args)..., '\n');
	}


	template <typename... Ts>
	inline std::ostream& warnln(Ts&&... args) {
		return fluke::warn(std::forward<Ts&&>(args)..., '\n');
	}


	template <typename... Ts>
	inline std::ostream& errorln(Ts&&... args) {
		return fluke::error(std::forward<Ts&&>(args)..., '\n');
	}


	template <typename... Ts>
	inline std::ostream& successln(Ts&&... args) {
		return fluke::success(std::forward<Ts&&>(args)..., '\n');
	}




	// Emphasised prints.
	template <typename T, typename... Ts>
	inline std::ostream& print_em(T&& first, Ts&&... args) {
		return fluke::print(
			style::bold, first, style::reset, std::forward<Ts&&>(args)...
		);
	}


	template <typename T, typename... Ts>
	inline std::ostream& notice_em(T&& first, Ts&&... args) {
		return fluke::notice(
			style::bold, first, style::reset, std::forward<Ts&&>(args)...
		);
	}


	template <typename T, typename... Ts>
	inline std::ostream& warn_em(T&& first, Ts&&... args) {
		return fluke::warn(
			style::bold, first, style::reset, std::forward<Ts&&>(args)...
		);
	}


	template <typename T, typename... Ts>
	inline std::ostream& error_em(T&& first, Ts&&... args) {
		return fluke::error(
			style::bold, first, style::reset, std::forward<Ts&&>(args)...
		);
	}


	template <typename T, typename... Ts>
	inline std::ostream& success_em(T&& first, Ts&&... args) {
		return fluke::success(
			style::bold, first, style::reset, std::forward<Ts&&>(args)...
		);
	}





	// Emphasis with newlines.
	template <typename... Ts>
	inline std::ostream& println_em(Ts&&... args) {
		return fluke::print_em(std::forward<Ts&&>(args)..., '\n');
	}


	template <typename... Ts>
	inline std::ostream& noticeln_em(Ts&&... args) {
		return fluke::notice_em(std::forward<Ts&&>(args)..., '\n');
	}


	template <typename... Ts>
	inline std::ostream& warnln_em(Ts&&... args) {
		return fluke::warn_em(std::forward<Ts&&>(args)..., '\n');
	}


	template <typename... Ts>
	inline std::ostream& errorln_em(Ts&&... args) {
		return fluke::error_em(std::forward<Ts&&>(args)..., '\n');
	}


	template <typename... Ts>
	inline std::ostream& successln_em(Ts&&... args) {
		return fluke::success_em(std::forward<Ts&&>(args)..., '\n');
	}





	// Headings.
	template <typename... Ts>
	inline std::ostream& print_h(Ts&&... args) {
		return fluke::print(style::bold, std::forward<Ts&&>(args)...);
	}


	template <typename... Ts>
	inline std::ostream& notice_h(Ts&&... args) {
		return fluke::notice(style::bold, std::forward<Ts&&>(args)...);
	}


	template <typename... Ts>
	inline std::ostream& warn_h(Ts&&... args) {
		return fluke::warn(style::bold, std::forward<Ts&&>(args)...);
	}


	template <typename... Ts>
	inline std::ostream& error_h(Ts&&... args) {
		return fluke::error(style::bold, std::forward<Ts&&>(args)...);
	}


	template <typename... Ts>
	inline std::ostream& success_h(Ts&&... args) {
		return fluke::success(style::bold, std::forward<Ts&&>(args)...);
	}




	// Headings with newlines.
	template <typename... Ts>
	inline std::ostream& println_h(Ts&&... args) {
		return fluke::print_h(std::forward<Ts&&>(args)..., '\n');
	}


	template <typename... Ts>
	inline std::ostream& noticeln_h(Ts&&... args) {
		return fluke::notice_h(std::forward<Ts&&>(args)..., '\n');
	}


	template <typename... Ts>
	inline std::ostream& warnln_h(Ts&&... args) {
		return fluke::warn_h(std::forward<Ts&&>(args)..., '\n');
	}


	template <typename... Ts>
	inline std::ostream& errorln_h(Ts&&... args) {
		return fluke::error_h(std::forward<Ts&&>(args)..., '\n');
	}


	template <typename... Ts>
	inline std::ostream& successln_h(Ts&&... args) {
		return fluke::success_h(std::forward<Ts&&>(args)..., '\n');
	}
}
