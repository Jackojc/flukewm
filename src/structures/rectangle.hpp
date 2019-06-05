#pragma once

namespace fluke {
	struct Rectangle {
		int x, y, w, h;
	};
}


std::ostream& operator<<(std::ostream& os, const fluke::Rectangle& rect) {
	const auto& [x, y, w, h] = rect;
	return (os << '(' << x << ", " << y << ", " << w << ", " << h << ')');
}
