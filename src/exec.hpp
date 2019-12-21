#pragma once

#include <string_view>
#include <array>
#include <sstream>

extern "C" {
#include <unistd.h>
}


namespace fluke {
	template <typename... Ts>
	void forkexec(std::string_view path, Ts&&... args) {
		if (fork() == 0) {
			setsid();

			// turn argument into a string using operator<< overload.
			std::stringstream ss;
	        auto empty = std::string{};

			const auto convert = [&] (const auto& x) {
				ss.str(empty);
				ss << x;
				return ss.str();
			};


			// argv
			std::array converted = { convert(std::forward<Ts>(args))... };
			std::array<char*, sizeof...(Ts) + 2> argv{};

			for (int i = 1; i < sizeof...(Ts) + 1; ++i)
	            argv.at(i) = converted.at(i - 1).data();


			argv.front() = const_cast<char*>(path.data());
			argv.back() = static_cast<char*>(nullptr);


			// environment variables.
			char* const envp[] = { static_cast<char*>(nullptr) };

			execve(path.data(), argv.data(), envp);
		}
	}
}
