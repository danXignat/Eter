#include "Board.h"

#include <array>

#include "IllusionService.h"
#include "logger.h"

using namespace logger;

namespace base {
	//----------------------------Constructors--------------------------------

	Board::Board(uint16_t size) :
		m_size{ size },
		m_bounding_rect{ size },
		m_available_spaces{ Coord{10, 5} } {

	}

	//----------------------------Public-Methods--------------------------------

	void Board::appendMove(const Coord& coord, CombatCard&& card) {
		auto top_card = this->getTopCard(coord);
		if (top_card.has_value() && top_card->get().isIllusion()) {
			IllusionService::event(top_card->get(), card);
		}
		else if (isValidMove(coord, card)) {
			m_bounding_rect.add(coord);

			m_combat_cards[coord].emplace_back(std::move(card));

			_updateAvailableSpaces(coord);

			Logger::log(
				Level::INFO,
				"card({}) appended at ({}, {})",
				combatCardToChar(m_combat_cards[coord].back().getType()), coord.first, coord.second
			);
		}
	}

	void Board::renderBoard() const {
		for (const auto& [coord, cards] : m_combat_cards) {
			utils::printAtCoordinate(cards.back(), coord.first, coord.second);
		}

		for (const auto& [x, y] : m_available_spaces) {
			utils::printAtCoordinate("*", x, y);
		}

	}

	std::optional<CombatCardRef> Board::getTopCard(Coord pos) {
		if (m_combat_cards.contains(pos)) {
			return std::ref(m_combat_cards[pos].back());
		}
		return std::nullopt;
	}

	Coord Board::getLeftCorner() const {
		return m_bounding_rect.corner1;
	}

	void Board::removeTopCardAt(const Coord& coord) {
		auto it = m_combat_cards.find(coord);

		if (it != m_combat_cards.end()) {
			// Inițializare sigură a card_stack
			auto& card_stack = it->second;

			card_stack.pop_back();

			Logger::log(Level::INFO, "Removed last card from the stack at ({}, {})", coord.first, coord.second);
		}
		else {
			Logger::log(Level::WARNING, "No stack or only one card present at ({}, {})", coord.first, coord.second);
		}
	}

	void Board::removeRow(const Coord& start_coord, const Player& player) {
		int row_count = 0;
		bool own_card_found = false;

		Coord current_coord = start_coord;

		while (m_combat_cards.find(current_coord) != m_combat_cards.end()) {
			auto& card_stack = m_combat_cards[current_coord];
			if (!card_stack.empty()) {
				if (card_stack.back().getColor() == player.getColor()) {
					own_card_found = true;
				}
				row_count++;
			}
			current_coord.first++; // Avansează pe linie
		}

		if (row_count >= 3 && own_card_found) {
			current_coord = start_coord;
			for (int i = 0; i < row_count; i++) {
				m_combat_cards.erase(current_coord);
				current_coord.first++;
			}
			Logger::log(Level::INFO, "Removed entire row starting from ({}, {})", start_coord.first, start_coord.second);
		}
		else {
			Logger::log(Level::WARNING, "Cannot remove row, conditions not met at ({}, {})", start_coord.first, start_coord.second);
		}
	}

	void Board::removeColumn(const Coord& start_coord, const Player& player) {
		int column_count = 0;
		bool own_card_found = false;

		Coord current_coord = start_coord;

		while (m_combat_cards.find(current_coord) != m_combat_cards.end()) {
			auto& card_stack = m_combat_cards[current_coord];
			if (!card_stack.empty()) {
				if (card_stack.back().getColor() == player.getColor()) {
					own_card_found = true;
				}
				column_count++;
			}
			current_coord.second++; // Avansează pe coloană
		}

		if (column_count >= 3 && own_card_found) {
			current_coord = start_coord;
			for (int i = 0; i < column_count; i++) {
				m_combat_cards.erase(current_coord);
				current_coord.second++;
			}
			Logger::log(Level::INFO, "Removed entire column starting from ({}, {})", start_coord.first, start_coord.second);
		}
		else {
			Logger::log(Level::WARNING, "Cannot remove column, conditions not met at ({}, {})", start_coord.first, start_coord.second);
		}
	}
	void Board::moveStack(const Coord& fromCoord, const Coord& toCoord, Player& player, bool isOpponent) {
		auto& combat_cards = m_combat_cards; 

		auto fromIt = combat_cards.find(fromCoord);

		if (fromIt != combat_cards.end() && !fromIt->second.empty()) {
			
			bool isCardValid = isOpponent
				? fromIt->second.back().getColor() != player.getColor()  // adversar
				: fromIt->second.back().getColor() == player.getColor(); // propriu

			if (isCardValid) {
				if (combat_cards.find(toCoord) == combat_cards.end()) {
					combat_cards[toCoord] = std::move(fromIt->second); 
					combat_cards.erase(fromIt); 
					Logger::log(Level::INFO, "Moved stack from ({}, {}) to ({}, {})", fromCoord.first, fromCoord.second, toCoord.first, toCoord.second);
				}
				else {
					Logger::log(Level::WARNING, "Destination position ({}, {}) is not empty", toCoord.first, toCoord.second);
				}
			}
			else {
				Logger::log(Level::WARNING, "Top card at original position ({}, {}) is not the correct team", fromCoord.first, fromCoord.second);
			}
		}
		else {
			Logger::log(Level::WARNING, "No stack found at the original position ({}, {})", fromCoord.first, fromCoord.second);
		}
	}


	//------------------------------Setter Getter------------------------------------
	uint16_t Board::size() const {
		return m_size;
	}

	bool Board::isFixed() const {
		return m_bounding_rect.isFixed();
	}


	uint16_t Board::getSize() const {
		return m_size;
	}

	//----------------------------Private Methods--------------------------------
	bool Board::isValidMove(const Coord& coord, const CombatCard& card) {
		if (m_combat_cards.contains(coord)) {
			bool bigger = card.getType() > m_combat_cards[coord].back().getType();
			
			if (!bigger) {
				Logger::log(Level::WARNING, "card too small");
				return false;
			}
		}
		else if (!m_available_spaces.contains(coord)) {
			Logger::log(Level::WARNING, "not available place");
			return false;
		}

		return true;
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

		if (m_bounding_rect.fixed_width) {
			std::erase_if(m_available_spaces, [&](Coord coord) {
				return !m_bounding_rect.withinWidth(coord);
				});
		}

		if (m_bounding_rect.fixed_height) {
			std::erase_if(m_available_spaces, [&](Coord coord) {
				return !m_bounding_rect.withinHeight(coord);
				});
		}
	}

	//--------------------------------Inner Classes-------------------------------------

	Board::BoundingRect::BoundingRect(uint16_t size) :
		size{ size },
		corner1{ 10, 5 }, corner2{ 10, 5 },
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

		fixed_width = ((corner2.first - corner1.first) / 2 + 1 == size) ? true : false;
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

	bool Board::BoundingRect::isFixed() const {
		return fixed_width && fixed_height;
	}


	///-----------------------------------------Iterator-------------------------------

	Board::iterator Board::begin() {
		return m_combat_cards.begin();
	}

	Board::iterator Board::end() {
		return m_combat_cards.end();
	}

	Board::const_iterator Board::begin() const {
		return m_combat_cards.begin();
	}

	Board::const_iterator Board::end() const {
		return m_combat_cards.end();
	}

}