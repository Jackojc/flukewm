#include <iostream>
#include <vector>
#include <string_view>
#include <array>
#include <algorithm>
#include <cstdlib>

#include <tinge.hpp>
#include <fluke.hpp>

enum {
	WINDOW_TELEPORT,
	WINDOW_RESIZE,
	WINDOW_MOVE,
	WINDOW_GROW,
};

constexpr std::pair<std::string_view, int> action_type[] = {

	{ "wtp", WINDOW_TELEPORT },
	{ "wrs", WINDOW_RESIZE   },
	{ "wmv", WINDOW_MOVE     },
	{ "wgr", WINDOW_GROW     },

};


int main(int argc, const char* argv[]) {

	std::vector<std::vector<std::string_view>> args;

	args.reserve(argc);
	for (auto& v: args)
		v.reserve(argc);

	for (int i = 1; i < argc; ++i) {
		auto val = argv[i];
		auto action = std::find_if(std::begin(action_type), std::end(action_type), [&val] (auto& p) {
			return val == p.first;
		});

		if (action == std::end(action_type)) {
			tinge::errorln("unknown action '", action, "'!");
			return 1;
		}

		auto [str, type] = *action;

		switch (type) {


		}


	}



	fluke::Connection conn;




	conn.sync();

	return 0;
}
