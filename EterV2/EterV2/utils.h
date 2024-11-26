#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <regex>

#include "CombatCard.h"

#include "typedefs.h"

namespace utils {
	struct CoordFunctor {
		size_t operator()(const base::Coord& coord) const {
			std::hash<uint16_t> hasher;

			return hasher(coord.first) ^ hasher(coord.second << 1);
		}
	};

	inline base::Coord operator+(const base::Coord& lhs, const base::Coord& rhs) {
		return { lhs.first + rhs.first, lhs.second + rhs.second };
	}

	inline void printAtCoordinate(std::string_view content, uint16_t x = 0, uint16_t y = 10) {
		std::cout << "\033[" << y << ";" << x << "H" << content;
		std::cout << "\033[" << 10 << ";" << 0 << "H" << std::flush;
	}

	inline void printAtCoordinate(std::string_view content, base::Coord coord) {
		std::cout << "\033[" << coord.second << ";" << coord.first << "H" << content;
		std::cout << "\033[" << 10 << ";" << 0 << "H" << std::flush;
	}

	inline void printAtCoordinate(const base::CombatCard& card, uint16_t x, uint16_t y) {
		std::cout << "\033[" << y << ";" << x << "H" << card;
		std::cout << "\033[" << 10 << ";" << 0 << "H" << std::flush;
	}

	inline bool isValidPos(const std::string& str) {
		std::regex regular_exp{ R"(\d{0,2})" };

		return std::regex_match(str, regular_exp);
	}
}