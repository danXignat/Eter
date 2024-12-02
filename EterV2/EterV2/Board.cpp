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

	Board::Board(GameSizeType size_type) :
		m_size{ static_cast<uint16_t>(size_type) },
		m_bounding_rect{ static_cast<uint16_t>(size_type) },
		m_available_spaces{Board::START_POS} {

	}

	Board::Board(Board&& other) noexcept {
		m_size = other.m_size;
		m_bounding_rect = other.m_bounding_rect;
		m_combat_cards = std::move(other.m_combat_cards);
		m_available_spaces = std::move(other.m_available_spaces);

		other.m_size = 0;
		other.m_bounding_rect = BoundingRect();
		other.m_combat_cards.clear();
		other.m_available_spaces.clear();
	}

	Board& Board::operator=(Board&& other) noexcept {
		if (&other != this) {
			m_size = other.m_size;
			m_bounding_rect = other.m_bounding_rect;
			m_combat_cards = std::move(other.m_combat_cards);
			m_available_spaces = std::move(other.m_available_spaces);

			other.m_size = 0;
			other.m_bounding_rect = BoundingRect();
			other.m_combat_cards.clear();
			other.m_available_spaces.clear();
		}

		return *this;
	}

	//----------------------------Public-Methods--------------------------------

	void Board::appendMove(const Coord& coord, CombatCard&& card) {
		auto top_card = this->getTopCard(coord);
		if (top_card.has_value() && top_card->get().isIllusion()) {
			IllusionService::event(top_card->get(), card);
		}
		m_bounding_rect.add(coord);

		m_combat_cards[coord].emplace_back(std::move(card));

		_updateAvailableSpaces(coord);

		Logger::log(
			Level::INFO,
			"card({}) appended at ({}, {})",
			combatCardToChar(m_combat_cards[coord].back().getType()), coord.first, coord.second
		);
	}

	bool Board::isValidMove(const Coord& coord, const CombatCard& card, bool bury) {
		if (m_combat_cards.contains(coord)) {

			if (card.getType() == CombatCardType::ETER || card.getType() == CombatCardType::HOLE) {
				Logger::log(Level::WARNING, "This card must be played on an empty space");
			}

			if (m_combat_cards[coord].back().getType() == CombatCardType::ETER) {
				Logger::log(Level::WARNING, "Cannot place card on top of an ETER card");
				return false;
			}
			if (m_combat_cards[coord].back().getType() == CombatCardType::HOLE) {
				Logger::log(Level::WARNING, "There's a hole!");
				return false;
			}

			bool bigger = card.getType() > m_combat_cards[coord].back().getType();
			auto top_card = this->getTopCard(coord);
			bool illusion = false;
			if (top_card.has_value() && top_card->get().isIllusion()) {
				illusion = true;
			}
			if (!bigger && !bury && !illusion) {
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

	void Board::render() const {
		for (const auto& [coord, cards] : m_combat_cards) {
			utils::printAtCoordinate(cards.back(), coord.first, coord.second);
		}

		for (const auto& [x, y] : m_available_spaces) {
			utils::printAtCoordinate("*", x, y);
		}

	}
	void Board::sideViewRender() {
		Coord print_pos{ 30, 1 };

		std::vector<std::pair<Coord, std::vector<CombatCardRef>>> cards;

		for (auto& [coord, stack] : m_combat_cards) {
			std::vector<CombatCardRef> card_refs;
			
			for (CombatCard& card : stack) {
				card_refs.emplace_back(card);
			}

			cards.emplace_back(coord, card_refs);
		}


		std::sort(cards.begin(), cards.end(), [](const auto& el1, const auto& el2) {
			return el1.first < el2.first; // Sort based on coordinates
			});

		utils::printAtCoordinate("List of cards", print_pos.first, print_pos.second);
		uint16_t i = 1;
		for (const auto& [coord, stack] : cards) {
			std::string pos = std::format("({}, {}) -> ", coord.first, coord.second);

			utils::printAtCoordinate(pos, print_pos.first, print_pos.second + i);
			uint16_t j = 0;
			for (const auto& card : stack) {
				utils::printAtCoordinate(card.get(),  // Dereference the unique_ptr
					print_pos.first + pos.size() + j,
					print_pos.second + i
				);
				j += 2;
			}

			i++;
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
		
			if (card_stack.empty()) {
				m_combat_cards.erase(it);
				_reinitialise();
			}

			Logger::log(Level::INFO, "Removed last card from the stack at ({}, {})", coord.first, coord.second);
		}
		else {
			Logger::log(Level::WARNING, "No stack or only one card present at ({}, {})", coord.first, coord.second);
		}
	}
	void Board::removeCardFromStackAt(const Coord& coord, const CombatCard& card_to_remove) {
		auto it = m_combat_cards.find(coord);

		if (it != m_combat_cards.end()) {
			auto& card_stack = it->second;
			auto card_it = std::find(card_stack.begin(), card_stack.end(), card_to_remove);
			if (card_it != card_stack.end()) {
				card_stack.erase(card_it);
				if (card_stack.empty()) {
					m_combat_cards.erase(it);
					_reinitialise(); 
				}

				Logger::log(Level::INFO, "Removed card from the stack at ({}, {})", coord.first, coord.second);
			}
			else {
				Logger::log(Level::WARNING, "Card not found in the stack at ({}, {})", coord.first, coord.second);
			}
		}
		else {
			Logger::log(Level::WARNING, "No stack found at ({}, {})", coord.first, coord.second);
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

	std::vector<Coord> Board::availableSpaces() {
		std::vector<Coord> choices;
		for (const auto& coord : m_available_spaces) {
			choices.emplace_back(coord);
		}
		return choices;
	}

	std::optional<std::vector<std::vector<Coord>>> Board::getBorders() const {
		std::vector<std::vector<Coord>> borders;

		auto addBorderIfValid = [&borders](std::vector<Coord>& border) {
			if (border.size() >= 3) {
				if (std::find(borders.begin(), borders.end(), border) == borders.end()) {
					borders.push_back(std::move(border));
				}
			}
		};

		std::vector<Coord> top_border, bottom_border;
		for (uint16_t point_x = m_bounding_rect.corner1.first; point_x <= m_bounding_rect.corner2.first; point_x += 2) {
			if (m_combat_cards.contains({ point_x, m_bounding_rect.corner1.second })) {
				top_border.push_back({ point_x, m_bounding_rect.corner1.second });
			}
			if (m_combat_cards.contains({ point_x, m_bounding_rect.corner2.second })) {
				bottom_border.push_back({ point_x, m_bounding_rect.corner2.second });
			}
		}
		addBorderIfValid(top_border);
		addBorderIfValid(bottom_border);

		std::vector<Coord> left_border, right_border;
		for (uint16_t point_y = m_bounding_rect.corner1.second; point_y <= m_bounding_rect.corner2.second; point_y++) {
			if (m_combat_cards.contains({ m_bounding_rect.corner1.first, point_y })) {
				left_border.push_back({ m_bounding_rect.corner1.first, point_y });
			}
			if (m_combat_cards.contains({ m_bounding_rect.corner2.first, point_y })) {
				right_border.push_back({ m_bounding_rect.corner2.first, point_y });
			}
		}
		addBorderIfValid(left_border);
		addBorderIfValid(right_border);

		return borders.empty() ? std::nullopt : std::make_optional(borders);
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

	///--------------------------------Inner Classes-------------------------------------

	Board::BoundingRect::BoundingRect()
		: size{ 0 },
		corner1{ 0, 0 }, corner2{ 0, 0 },
		fixed_width{ false }, fixed_height{ false } {

	}

	Board::BoundingRect::BoundingRect(uint16_t size) :
		size{ size },
		corner1{ 10, 5 }, corner2{ 10, 5 },
		fixed_width{ false }, fixed_height{ false } {

	}

	Board::BoundingRect::BoundingRect(uint16_t size, const Coord& coord) :
		size{ size },
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