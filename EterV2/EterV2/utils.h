#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <regex>

#include "Config.h"
#include "CombatCard.h"

namespace base {
	namespace utils {
		struct CoordFunctor {
			size_t operator()(const Coord& coord) const {
				std::hash<uint16_t> hasher;

				return hasher(coord.first) ^ hasher(coord.second << 1);
			}
		};

		inline void printAtCoordinate(std::string_view content, uint16_t x = 0, uint16_t y = 10) {
			std::cout << "\033[" << y << ";" << x << "H" << content;
			std::cout << "\033[" << 22 << ";" << 0 << "H" << std::flush;
		}

		inline void printAtCoordinate(std::string_view content, Coord coord) {
			std::cout << "\033[" << coord.second << ";" << coord.first << "H" << content;
			std::cout << "\033[" << 22 << ";" << 0 << "H" << std::flush;
		}

		inline void printAtCoordinate(const CombatCard& card, uint16_t x, uint16_t y) {
			std::cout << "\033[" << y << ";" << x << "H" << card;
			std::cout << "\033[" << 22 << ";" << 0 << "H" << std::flush;
		}

		inline bool isValidPos(const std::string& str) {
			std::regex regular_exp{ R"(\d{0,2})" };

			return std::regex_match(str, regular_exp);
		}

		inline Coord operator+(const Coord& lhs, const Coord& rhs) {
			return { lhs.first + rhs.first, lhs.second + rhs.second };
		}

		inline Coord operator-(const Coord& lhs, const Coord& rhs) {
			return { lhs.first - rhs.first, lhs.second - rhs.second };
		}

		inline Coord operator*(int16_t scalar, const Coord& rhs) {
			return { rhs.first * scalar, rhs.second * scalar };
		}

		inline Coord operator/(const Coord& lhs, const Coord& rhs) {
			return { lhs.first / rhs.first, lhs.second / rhs.second };
		}

		inline Coord mapCoordToMatrix(const Coord& top_left_corner, const Coord& coord) {
			Config& config{ Config::getInstance() };

			Coord translated{ coord - top_left_corner };
			Coord scaled{ translated / config.getSpacing() };
			Coord from_xy_to_rowcol{ scaled.second, scaled.first };

			return from_xy_to_rowcol;
		}
	}
}
