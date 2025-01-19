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
	/*inline ColorType from_string(const std::string& color_str) {
		if (color_str == "RED") return ColorType::RED;
		if (color_str == "BLUE") return ColorType::BLUE;
		if (color_str == "DEFAULT") return ColorType::DEFAULT;
		throw std::invalid_argument("Invalid color string: " + color_str);
	}*/
}
