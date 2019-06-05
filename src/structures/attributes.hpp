#pragma once

#include <iostream>

namespace fluke {
	struct Attributes {
		int16_t x, y;
		uint16_t w, h, border_width;
		bool ignored, mapped;
	};
}


std::ostream& operator<<(std::ostream& os, const fluke::Attributes& attr) {
	const auto& [x, y, w, h, border, ignored, mapped] = attr;

	return (
		os << "((" << x << ", " << y << ", " << w << ", " << h << "), bor: " <<
		border << ", ign: " << ignored << ", map: " << mapped << ')'
	);
}
