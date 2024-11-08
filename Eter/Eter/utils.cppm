export module utils;

import <iostream>;

import CombatCard;

namespace utils {
	export void printAtCoordinate(std::string_view content, uint16_t x = 0, uint16_t y = 10) {
		std::cout << "\033[" << y << ";" << x << "H" << content;
		std::cout << "\033[" << 10 << ";" << 0 << "H" << std::flush;
	}

	export void printAtCoordinate(const base::CombatCard& card, uint16_t x, uint16_t y) {
		std::cout << "\033[" << y << ";" << x << "H" << card;
		std::cout << "\033[" << 10 << ";" << 0 << "H" << std::flush;
	}
}