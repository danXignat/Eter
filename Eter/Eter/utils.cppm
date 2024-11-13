export module utils;

import <iostream>;
import <string>;
import <unordered_map>;

import CombatCard;

namespace utils {
	export using Coord = std::pair<int16_t, int16_t>;

	export struct CoordFunctor {
		size_t operator()(const Coord& coord) const {
			std::hash<uint16_t> hasher;

			return hasher(coord.first) ^ hasher(coord.second << 1);
		}
	};

	Coord operator+(const Coord& lhs, const Coord& rhs) {
		return { lhs.first + rhs.first, lhs.second + rhs.second };
	}

	export void printAtCoordinate(std::string_view content, uint16_t x = 0, uint16_t y = 10) {
		std::cout << "\033[" << y << ";" << x << "H" << content;
		std::cout << "\033[" << 10 << ";" << 0 << "H" << std::flush;
	}

	export void printAtCoordinate(std::string_view content, Coord coord) {
		std::cout << "\033[" << coord.second << ";" << coord.first << "H" << content;
		std::cout << "\033[" << 10 << ";" << 0 << "H" << std::flush;
	}

	export void printAtCoordinate(const base::CombatCard& card, uint16_t x, uint16_t y) {
		std::cout << "\033[" << y << ";" << x << "H" << card;
		std::cout << "\033[" << 10 << ";" << 0 << "H" << std::flush;
	}

	export bool isNumber(const std::string& str) {
		try {
			std::size_t pos;
			std::stod(str, &pos);
			return pos == str.size();
		}
		catch (const std::invalid_argument&) {
			return false;
		}
		catch (const std::out_of_range&) {
			return false;
		}
	}

}