#pragma once

namespace fluke {
	struct Point {
		int x, y;
	};
}


std::ostream& operator<<(std::ostream& os, const fluke::Point& point) {
	const auto& [x, y] = point;
	return (os << '(' << x << ", " << y << ')');
}

