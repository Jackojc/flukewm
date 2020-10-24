#ifndef FLUKE_KEYS_HPP
#define FLUKE_KEYS_HPP

#pragma once

#include <array>
#include <vector>
#include <fluke.hpp>

// extern "C" {
// 	#include <X11/keysym.h>
// 	#include <X11/XF86keysym.h>
// }


namespace fluke {

	using KeyCallback = void(*)(fluke::Connection&);

	struct Key {
		unsigned mod;
		xcb_keysym_t keysym;
		fluke::KeyCallback func;
	};


	// Basically an array with a known T.
	template <size_t N>
	struct Keys: std::array<fluke::Key, N> {};

	// Deduction guide so we can automatically determine
	// the size of the array.
	template <class... Ts>
	Keys(Ts...) -> Keys<sizeof...(Ts)>;




	// Keysyms
	namespace keys {
		constexpr uint32_t alt         = XCB_MOD_MASK_1;
		constexpr uint32_t num_lock    = XCB_MOD_MASK_2;
		constexpr uint32_t altgr       = XCB_MOD_MASK_3;
		constexpr uint32_t super       = XCB_MOD_MASK_4;
		constexpr uint32_t scroll_lock = XCB_MOD_MASK_5;
		constexpr uint32_t control     = XCB_MOD_MASK_CONTROL;
		constexpr uint32_t shift       = XCB_MOD_MASK_SHIFT;
		constexpr uint32_t caps_lock   = XCB_MOD_MASK_LOCK;

		constexpr uint32_t ret       = 0xff0d;
		constexpr uint32_t backspace = 0xff08;
		constexpr uint32_t tab       = 0xff09;
		constexpr uint32_t escape    = 0xff1b;

		constexpr uint32_t print = 0xff61;
		constexpr uint32_t del   = 0xffff;

		constexpr uint32_t up    = 0xff52;
		constexpr uint32_t down  = 0xff54;
		constexpr uint32_t left  = 0xff51;
		constexpr uint32_t right = 0xff53;

		constexpr uint32_t next  = 0xff56;
		constexpr uint32_t prior = 0xff55;

		constexpr uint32_t pageup   = 0xff55;
		constexpr uint32_t pagedown = 0xff56;

		constexpr uint32_t home  = 0xff50;
		constexpr uint32_t end   = 0xff57;
		constexpr uint32_t begin = 0xff58;

		constexpr uint32_t insert = 0xff63;

		constexpr uint32_t comma  = 0x002c;
		constexpr uint32_t period = 0x002e;
		constexpr uint32_t space  = 0x0020;
		constexpr uint32_t minus  = 0x002d;

		constexpr uint32_t slash     = 0x002f;
		constexpr uint32_t backslash = 0x005c;

		constexpr uint32_t semicolon = 0x003b;
		constexpr uint32_t equal     = 0x003d;

		constexpr uint32_t bracketleft  = 0x005b;
		constexpr uint32_t bracketright = 0x005d;
		constexpr uint32_t braceleft    = 0x007b;
		constexpr uint32_t braceright   = 0x007d;

		constexpr uint32_t function_1  = 0xffbe;
		constexpr uint32_t function_2  = 0xffbf;
		constexpr uint32_t function_3  = 0xffc0;
		constexpr uint32_t function_4  = 0xffc1;
		constexpr uint32_t function_5  = 0xffc2;
		constexpr uint32_t function_6  = 0xffc3;
		constexpr uint32_t function_7  = 0xffc4;
		constexpr uint32_t function_8  = 0xffc5;
		constexpr uint32_t function_9  = 0xffc6;
		constexpr uint32_t function_10 = 0xffc7;
		constexpr uint32_t function_11 = 0xffc8;
		constexpr uint32_t function_12 = 0xffc9;
		constexpr uint32_t function_13 = 0xffca;
		constexpr uint32_t function_14 = 0xffcb;
		constexpr uint32_t function_15 = 0xffcc;
		constexpr uint32_t function_16 = 0xffcd;
		constexpr uint32_t function_17 = 0xffce;
		constexpr uint32_t function_18 = 0xffcf;
		constexpr uint32_t function_19 = 0xffd0;
		constexpr uint32_t function_20 = 0xffd1;
		constexpr uint32_t function_21 = 0xffd2;
		constexpr uint32_t function_22 = 0xffd3;
		constexpr uint32_t function_23 = 0xffd4;
		constexpr uint32_t function_24 = 0xffd5;
		constexpr uint32_t function_25 = 0xffd6;
		constexpr uint32_t function_26 = 0xffd7;
		constexpr uint32_t function_27 = 0xffd8;
		constexpr uint32_t function_28 = 0xffd9;
		constexpr uint32_t function_29 = 0xffda;
		constexpr uint32_t function_30 = 0xffdb;
		constexpr uint32_t function_31 = 0xffdc;
		constexpr uint32_t function_32 = 0xffdd;
		constexpr uint32_t function_33 = 0xffde;
		constexpr uint32_t function_34 = 0xffdf;
		constexpr uint32_t function_35 = 0xffe0;

		constexpr uint32_t number_0 = 0x0030;
		constexpr uint32_t number_1 = 0x0031;
		constexpr uint32_t number_2 = 0x0032;
		constexpr uint32_t number_3 = 0x0033;
		constexpr uint32_t number_4 = 0x0034;
		constexpr uint32_t number_5 = 0x0035;
		constexpr uint32_t number_6 = 0x0036;
		constexpr uint32_t number_7 = 0x0037;
		constexpr uint32_t number_8 = 0x0038;
		constexpr uint32_t number_9 = 0x0039;

		constexpr uint32_t a = 0x0061;
		constexpr uint32_t b = 0x0062;
		constexpr uint32_t c = 0x0063;
		constexpr uint32_t d = 0x0064;
		constexpr uint32_t e = 0x0065;
		constexpr uint32_t f = 0x0066;
		constexpr uint32_t g = 0x0067;
		constexpr uint32_t h = 0x0068;
		constexpr uint32_t i = 0x0069;
		constexpr uint32_t j = 0x006a;
		constexpr uint32_t k = 0x006b;
		constexpr uint32_t l = 0x006c;
		constexpr uint32_t m = 0x006d;
		constexpr uint32_t n = 0x006e;
		constexpr uint32_t o = 0x006f;
		constexpr uint32_t p = 0x0070;
		constexpr uint32_t q = 0x0071;
		constexpr uint32_t r = 0x0072;
		constexpr uint32_t s = 0x0073;
		constexpr uint32_t t = 0x0074;
		constexpr uint32_t u = 0x0075;
		constexpr uint32_t v = 0x0076;
		constexpr uint32_t w = 0x0077;
		constexpr uint32_t x = 0x0078;
		constexpr uint32_t y = 0x0079;
		constexpr uint32_t z = 0x007a;

		constexpr uint32_t monitor_brightness_up    = 0x1008FF02;
		constexpr uint32_t monitor_brightness_down  = 0x1008FF03;
		constexpr uint32_t monitor_brightness_cycle = 0x1008FF07;

		constexpr uint32_t touchpad_toggle = 0x1008FFA9;
		constexpr uint32_t touchpad_on     = 0x1008FFB0;
		constexpr uint32_t touchpad_off    = 0x1008FFB1;

		constexpr uint32_t audio_lower_volume = 0x1008FF11;
		constexpr uint32_t audio_raise_volume = 0x1008FF13;
		constexpr uint32_t audio_mic_mute     = 0x1008FFB2;
		constexpr uint32_t audio_mute         = 0x1008FF12;
		constexpr uint32_t audio_play         = 0x1008FF14;
		constexpr uint32_t audio_pause        = 0x1008FF31;
		constexpr uint32_t audio_stop         = 0x1008FF15;
		constexpr uint32_t audio_prev         = 0x1008FF16;
		constexpr uint32_t audio_next         = 0x1008FF17;
		constexpr uint32_t audio_forward      = 0x1008FF97;
		constexpr uint32_t audio_repeat       = 0x1008FF98;
		constexpr uint32_t audio_random_play  = 0x1008FF99;

		constexpr uint32_t suspend     = 0x1008FFA7;
		constexpr uint32_t hibernate   = 0x1008FFA8;
		constexpr uint32_t log_off     = 0x1008FF61;
		constexpr uint32_t standby     = 0x1008FF10;
		constexpr uint32_t power_off   = 0x1008FF2A;
		constexpr uint32_t wake_up     = 0x1008FF2B;
		constexpr uint32_t eject       = 0x1008FF2C;
		constexpr uint32_t screensaver = 0x1008FF2D;
		constexpr uint32_t sleep       = 0x1008FF2F;
	}
}

#endif
