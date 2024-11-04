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

	Board::Board() {

	}

	Board::Board(uint16_t size) :
		m_size{ size },
		m_col_min{ SHRT_MAX }, m_col_max{ SHRT_MIN },
		m_row_min{ SHRT_MAX }, m_row_max{ SHRT_MIN } {

	}

	//----------------------------Public-Methods--------------------------------

	void Board::appendMove(Coord coord, CardPtr&& card_ptr) {
		_updateBoardBoundry(coord);

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
	}

	//----------------------------Private-Methods--------------------------------

	void Board::_updateBoardBoundry(Coord coord) {
		m_row_min = std::min(int(m_row_min), int(coord.first));
		m_row_max = std::max(int(m_row_max), int(coord.first));
		m_col_min = std::min(int(m_col_min), int(coord.second));
		m_col_max = std::max(int(m_col_max), int(coord.second));
	}

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

		if (m_row_max - m_row_min) {

		}
		if (m_col_max - m_col_min) {

		}
	}

	//--------------------------------Functor-------------------------------------

	size_t Board::CoordFunctor::operator()(const Coord& coord) const {
		std::hash<uint16_t> hasher;

		return hasher(coord.first) ^ hasher(coord.second << 1);
	}
}