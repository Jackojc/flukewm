#pragma once

extern "C" {
#include <unistd.h>
}

namespace fluke {
	/*
		Launches a program specified by first argument,
		remaining arguments are passed to the new processes
		argv[].

		example:
			if (exec("script", "a", "b"))
				std::cout << "Success!\n";
			else
				std::cout << "Error!\n";
	*/
	template <typename... Ts>
	bool exec(const char* arg, Ts&&... args) {
		FLUKE_DEBUG_NOTICE("run '", tinge::fg::make_yellow(arg), "'")

		if (fork() != 0)
			return false;

		if (setsid() == -1)
			return false;

		return execlp(arg, arg, args..., (char*)nullptr) != -1;
	}
}
