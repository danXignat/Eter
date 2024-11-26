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

	void Board::appendMove(const Coord& coord, CombatCard&& card,bool bury) {
		auto top_card = this->getTopCard(coord);
		if (top_card.has_value() && top_card->get().isIllusion()) {
			IllusionService::event(top_card->get(), card);
		}
		else if (_isValidMove(coord, card,bury)) {
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

	void Board::render() const {
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
			auto& card_stack = it->second;

			card_stack.pop_back();

			Logger::log(Level::INFO, "Removed last card from the stack at ({}, {})", coord.first, coord.second);
		}
		else {
			Logger::log(Level::WARNING, "No stack or only one card present at ({}, {})", coord.first, coord.second);
		}
	}

	void Board::removeStack(const Coord& coord) {
		auto it = m_combat_cards.find(coord);

		if (it != m_combat_cards.end()) {
			m_combat_cards.erase(it);

			_reinitialise();

			Logger::log(Level::INFO, "stack removed at ({}, {})", coord.first, coord.second);
		}
		else {
			Logger::log(Level::ERROR, "stack removed failed at ({}, {})", coord.first, coord.second);
		}
	}

	void Board::removeRow(uint16_t y) {
		if (m_bounding_rect.corner1.second <= y && y <= m_bounding_rect.corner2.second) {
			for (auto it = m_combat_cards.begin(); it != m_combat_cards.end(); ) {
				if (it->first.second == y) {
					it = m_combat_cards.erase(it);
				}
				else {
					it++;
				}
			}

			_reinitialise();
		}
		else {
			Logger::log(Level::ERROR, "no card on row{}", y);
		}
	}

	void Board::removeColumn(uint16_t x) {
		if (m_bounding_rect.corner1.first <= x && x <= m_bounding_rect.corner2.first) {
			for (auto it = m_combat_cards.begin(); it != m_combat_cards.end(); ) {
				if (it->first.first == x) {
					it = m_combat_cards.erase(it);
				}
				else {
					it++;
				}
			}

			_reinitialise();
		}
		else {
			Logger::log(Level::ERROR, "no card on row{}", x);
		}
	}

	void Board::moveStack(const Coord& from_coord, const Coord& to_coord) {
		auto it = m_combat_cards.find(from_coord);
		if (it == m_combat_cards.end()) {
			Logger::log(Level::ERROR, "No stack at ({},{})\n", from_coord.first, from_coord.second);
			return;
		}
		if (m_combat_cards.contains(to_coord)) {
			Logger::log(Level::ERROR, "Destination must be an empty space!\n", from_coord.first, from_coord.second);
			return;
		}
		if (!m_available_spaces.contains(to_coord)) {
			Logger::log(Level::ERROR, "Destination is not a valid space!\n", from_coord.first, from_coord.second);
			return;
		}

		m_combat_cards[to_coord] = std::move(it->second);
		m_combat_cards.erase(it);

		_reinitialise(); 

		Logger::log(Level::INFO, "Stack moved from ({}, {}) to ({}, {})",
			from_coord.first, from_coord.second, to_coord.first, to_coord.second);
	
	}
	

	void Board::swapStacks(const Coord& from_coord, const Coord& to_coord) {

	}

	//------------------------------Setter Getter------------------------------------

	uint16_t Board::size() const {
		return m_size;
	}

	bool Board::isFixed() const {
		return m_bounding_rect.isFixed();
	}

	bool Board::isCardOfColorAt(color::ColorType color, const Coord& coord) const {
		auto& stack = m_combat_cards.at(coord);

		auto is_color = [color](const CombatCard& card) { return card.getColor() == color; };

		auto it = std::find_if(stack.begin(), stack.end(), is_color);

		if (it != stack.end()) {
			return true;
		}

		return false;
	}

	uint16_t Board::getSize() const {
		return m_size;
	}

	//----------------------------Private Methods--------------------------------
	bool Board::_isValidMove(const Coord& coord, const CombatCard& card,bool bury) {
		if (m_combat_cards.contains(coord)) {
			bool bigger = card.getType() > m_combat_cards[coord].back().getType();
			
			if (!bigger&&!bury) {
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

	void Board::_reinitialise() {
		m_available_spaces.clear();

		if (m_combat_cards.empty()) {
			m_bounding_rect = BoundingRect(m_size);
			m_available_spaces.insert({ 10, 5 });
		}
		else {
			m_bounding_rect = BoundingRect(m_size, m_combat_cards.begin()->first);

			for (const auto& [coord, stack] : m_combat_cards) {
				m_bounding_rect.add(coord);

				_updateAvailableSpaces(coord);
			}
		}
	}

	//--------------------------------Inner Classes-------------------------------------

	Board::BoundingRect::BoundingRect(uint16_t size) :
		size{ size },
		corner1{ 10, 5 }, corner2{ 10, 5 },
		fixed_width{ false }, fixed_height{ false } {

	}

	Board::BoundingRect::BoundingRect(uint16_t size, const Coord& coord) :
		size{size},
		corner1{ coord.first, coord.second }, corner2{ coord.first, coord.second },
		fixed_width{ false }, fixed_height{ false } {

	}

	void Board::BoundingRect::add(const Coord& coord) {
		if (!fixed_width) {
			corner1.first = std::min(corner1.first, coord.first);
			corner2.first = std::max(corner2.first, coord.first);
		}

		if (!fixed_height) {
			corner1.second = std::min(corner1.second, coord.second);
			corner2.second = std::max(corner2.second, coord.second);
		}

		fixed_width = ((corner2.first - corner1.first) / 2 + 1 == size) ? true : false;
		fixed_height = (corner2.second - corner1.second + 1 == size) ? true : false;
	}

	void Board::BoundingRect::remove(const Coord& coord) {

	}

	bool Board::BoundingRect::withinWidth(const Coord& coord) const {
		return corner1.first <= coord.first && coord.first <= corner2.first;
	}

	bool Board::BoundingRect::withinHeight(const Coord& coord) const {
		return corner1.second <= coord.second && coord.second <= corner2.second;
	}

	bool Board::BoundingRect::within(const Coord& coord) const {
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