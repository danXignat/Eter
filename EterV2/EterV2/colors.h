#pragma once
#include <cstdint>
#include <string>

namespace color {
	enum class ColorType : uint8_t {
		RED,
		BLUE,
		DEFAULT
	};

	inline std::string to_string(ColorType color) {
		switch (color)
		{
			using enum ColorType;
		case RED:
			return "\033[31m";

		case BLUE:
			return "\033[34m";

		default:
			return "\033[0m";
		}
	}
}
