module Board;

import <iostream>;
import <memory>;
import <vector>;
import <stack>;
import <unordered_map>;
import <unordered_set>;

#include <array>
#include <limits.h>

import CombatCard;

namespace utils {
	void printAtCoordinate(uint16_t x, uint16_t y, std::string_view content) {
		std::cout << "\033[" << y << ";" << x << "H" << content;
		std::cout << "\033[" << 20 << ";" << 0 << "H";
	}

	void printAtCoordinate(uint16_t x, uint16_t y, const base::CombatCard& card) {
		std::cout << "\033[" << y << ";" << x << "H" << card << "\033[K";
		std::cout << "\033[" << 20 << ";" << 0 << "H";
	}
}

namespace base {
	//----------------------------Constructors--------------------------------

	Board::Board(uint16_t size) :
		m_size{ size },
		m_bounding_rect{ size } {

	}

	//----------------------------Public-Methods--------------------------------

	void Board::appendMove(Coord coord, CardPtr&& card_ptr) {
		m_bounding_rect.add(coord);

		m_combat_cards[coord].push_back(std::move(card_ptr));

		_updateAvailableSpaces(coord);
	}

	void Board::renderBoard() const {
		for (const auto& pair : m_combat_cards) {
			utils::printAtCoordinate(pair.first.first, pair.first.second, *pair.second.back());
		}

		for (const auto& pair : m_available_spaces) {
			utils::printAtCoordinate(pair.first, pair.second, "*");
		}

		for (int i = 0; i < m_bounding_rect.corner2.second - m_bounding_rect.corner1.second + 1; i++) {
			for (int j = 0; j < m_bounding_rect.corner2.first - m_bounding_rect.corner1.first + 1; j += 2)
				std::cout << "*";
			std::cout << std::endl;
		}
		std::cin.get();
	}

	//----------------------------Private-Methods--------------------------------


	void Board::_updateAvailableSpaces(Coord coord) {
		std::array<Coord, 8> offsets = { {
			{2, 0}, {2, -1}, {0, -1}, {-2, -1},
			{-2, 0}, {-2, 1}, {0, 1}, {2, 1}
		} };

		m_available_spaces.erase(coord);

		for (const auto& offset : offsets) {
			Coord new_point{ coord.first + offset.first, coord.second + offset.second };

			if (!m_combat_cards.contains(new_point)) {
				m_available_spaces.emplace(new_point);
			}
		}
	}

	//--------------------------------Inner-Classes-------------------------------------

	size_t Board::CoordFunctor::operator()(const Coord& coord) const {
		std::hash<uint16_t> hasher;

		return hasher(coord.first) ^ hasher(coord.second << 1);
	}

	Board::BoundingRect::BoundingRect(uint16_t size) :
		size{ size },
		corner1{ 10, 8 }, corner2{ 10, 8 },
		fixed_width{ false }, fixed_height{ false } {

	}

	void Board::BoundingRect::add(Coord coord) {
		if (!fixed_width) {
			corner1.first = std::min(corner1.first, coord.first);
			corner2.first = std::max(corner2.first, coord.first);
		}
		
		if (!fixed_height) {
			corner1.second = std::min(corner1.second, coord.second);
			corner2.second = std::max(corner2.second, coord.second);
		}

		/*std::cout << corner1.first << " " << corner1.second << std::endl;
		std::cout << corner2.first << " " << corner2.second << std::endl << std::endl;*/

		fixed_width = ( (corner2.first - corner1.first) /2 + 1 == size) ? true : false;
		fixed_height = (corner2.second - corner1.second + 1 == size) ? true : false;
	}

	void Board::BoundingRect::remove(Coord coord) {

	}

	bool Board::BoundingRect::withinWidth(Coord coord) const {
		return corner1.first <= coord.first && coord.first <= corner2.first;
	}

	bool Board::BoundingRect::withinHeight(Coord coord) const {
		return corner1.second <= coord.second && coord.second <= corner2.second;
	}

	bool Board::BoundingRect::within(Coord coord) const {
		return withinHeight(coord) && withinWidth(coord);
	}
}