#include <iostream>
#include <array>
#include <vector>
#include <string>
#include <map>

#include <tinge.hpp>
#include <fluke.hpp>

#include <utils/atomget.cpp>
#include <utils/atomset.cpp>
#include <utils/binpack.cpp>
#include <utils/center.cpp>
#include <utils/focus.cpp>
#include <utils/grow.cpp>
#include <utils/move.cpp>
#include <utils/shrink.cpp>



using callback = int(*)(fluke::Connection&, std::vector<std::string>&);

std::map<std::string_view, callback> funcs = {
	{ "atomget", fluke::utils::atomget },
	{ "atomset", fluke::utils::atomset },
	{ "binpack", fluke::utils::binpack },
	{ "center",  fluke::utils::center  },
	{ "focus",   fluke::utils::focus   },
	{ "grow",    fluke::utils::grow    },
	{ "move",    fluke::utils::move    },
	{ "shrink",  fluke::utils::shrink  },
};



// flukewm <action> [args...]

int main(int argc, const char* argv[]) {
	fluke::Connection conn;

	if (argc == 1) {
		tinge::errorln("specify an action!");
		return 1;
	}


	try {
		// See if a callback exists, if so, call it and pass connection & vector of args.
		std::vector<std::string> args{ argv + 2, argv + argc };
		return funcs.at(argv[1])(
			conn, args
		);

	} catch (const std::out_of_range&) {
		tinge::errorln("unknown action!");
		return 1;
	}


	return 0;
}
