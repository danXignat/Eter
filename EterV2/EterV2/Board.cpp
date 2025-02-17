#include "Board.h"

#include <queue>
#include <ranges>

#include "logger.h"

using namespace logger;

namespace base {
	//----------------------------Constructors--------------------------------

	Board::Board(GameSizeType size_type, Player& player1, Player& player2) :
		m_size{ static_cast<uint16_t>(size_type) },
		m_bounding_rect{ static_cast<uint16_t>(size_type) },
		m_available_spaces{ Config::getInstance().getStartPoint() },
		m_player1{ player1 }, m_player2{ player2 } {

	}

	//----------------------------MODIFIERS--------------------------------

	void Board::returnUsedCardToHand(CombatCardType card) {

		if (m_player1.hasUsedCard(card)) {
			CombatCard to_return = m_player1.getUsedCard(card);
			m_player1.addCard(std::move(to_return));
		}
		if (m_player2.hasUsedCard(card)) {
			CombatCard to_return = m_player2.getUsedCard(card);
			m_player2.addCard(std::move(to_return));
		}

	}

	void Board::returnCardToHand(CombatCard&& card, color::ColorType owner) {
		if (m_player1.getColor() == owner) {
			m_player1.addCard(std::move(card));
		}
		if (m_player2.getColor() == owner) {
			m_player2.addCard(std::move(card));
		}
	}

	void Board::createHole(const Coord& coord) {
		while (m_combat_cards[coord].size()) {
			CombatCard card{ std::move(m_combat_cards[coord].back()) };
			m_combat_cards[coord].pop_back();

			if (card.getColor() == color::ColorType::RED) {
				m_player1.addUsedCard(std::move(card));
			}
			else {
				m_player2.addUsedCard(std::move(card));
			}
		}

		m_combat_cards[coord].push_back(
			CombatCard{ CombatCardType::HOLE, color::ColorType::DEFAULT }
		);

		_reinitialise();

		Logger::log(Level::INFO, "Hole created at {} {}", coord.first, coord.second);
	}

	void Board::appendMove(const Coord& coord, CombatCard&& card) {
		if (this->isValidPlaceCard(coord, card) == false) {
			throw std::runtime_error("move not valid");
		}

		m_bounding_rect.add(coord);
		setLastPlayedCard(card, coord);

		if (m_incremented_card.has_value() && m_incremented_card->first == coord) {
			resetIncrementCardValue();
		}
		if (m_decremented_card.has_value() && m_decremented_card->first == coord) {
			resetDecrementedCard();
		}

		m_combat_cards[coord].emplace_back(std::move(card));

		_updateAvailableSpaces(coord);

		_setWinPosition(coord);

		Logger::log(
			Level::INFO,
			"card({}) appended at ({}, {})",
			combatCardToChar(m_combat_cards[coord].back().getType()), coord.first, coord.second
		);
	}

	void Board::moveStack(const Coord& from_coord, const Coord& to_coord) {
		if (m_combat_cards.contains(to_coord)) {
			throw std::runtime_error("Destination must be an empty space!");
		}

		if (!m_available_spaces.contains(to_coord)) {
			throw std::runtime_error("Destination is not a valid space!\n");
		}

		if (this->isValidMoveStack(from_coord, to_coord) == false) {
			throw std::runtime_error("Not conex anymore");
		}

		m_combat_cards[to_coord] = std::move(m_combat_cards[from_coord]);
		m_combat_cards.erase(from_coord);

		_reinitialise();

		Logger::log(Level::INFO, "Stack moved from ({}, {}) to ({}, {})",
			from_coord.first, from_coord.second, to_coord.first, to_coord.second);
	}

	void Board::moveStacks(const std::vector<std::pair<Coord, Coord>>& moves) {
		if (!isValidMoveStacks(moves)) {
			throw std::runtime_error("Invalid move");
		}
		for (const auto& [from, to] : moves) {
			m_combat_cards[to] = std::move(m_combat_cards[from]);
			m_combat_cards.erase(from);
		}

		_reinitialise();

		for (const auto& [from, to] : moves) {
			Logger::log(Level::INFO, "Stack moved from ({}, {}) to ({}, {})",
				from.first, from.second, to.first, to.second);
		}
	}
	void Board::shiftRowToRight(const std::vector<Coord>& row_coords) {
		std::vector<std::pair<Coord, Coord>> moves;

		for (auto it = row_coords.rbegin(); it != row_coords.rend(); ++it) {
			Coord from = *it;
			Coord to = { from.first + 2, from.second };
			moves.emplace_back(from, to);
		}

		moveStacks(moves);
	}
	void Board::shiftRowToLeft(const std::vector<Coord>& row_coords) {
		std::vector<std::pair<Coord, Coord>> moves;

		for (auto it = row_coords.begin(); it != row_coords.end(); ++it) {
			Coord from = *it;
			Coord to = { from.first - 2, from.second };
			moves.emplace_back(from, to);
		}
		moveStacks(moves);
	}
	void Board::shiftColumnDown(const std::vector<Coord>& col_coords) {
		std::vector<std::pair<Coord, Coord>> moves;

		for (auto it = col_coords.rbegin(); it != col_coords.rend(); ++it) {
			Coord from = *it;
			Coord to = { from.first , from.second + 1 };
			moves.emplace_back(from, to);
		}
		moveStacks(moves);
	}
	void Board::shiftColumnUp(const std::vector<Coord>& col_coords) {
		std::vector<std::pair<Coord, Coord>> moves;

		for (auto it = col_coords.begin(); it != col_coords.end(); ++it) {
			Coord from = *it;
			Coord to = { from.first, from.second - 1 };
			moves.emplace_back(from, to);
		}
		moveStacks(moves);
	}

	void Board::mergeStacks(const std::vector<Coord>& coords, const Coord& final_coord) {
		if (!isValidMergeStacks(coords, final_coord)) {
			Logger::log(Level::WARNING, "Merge stacks validation failed.");
			return;
		}
		bool forward = (final_coord == coords.back());
		auto merge = [&](const Coord& src, const Coord& dest) {
			if (!m_combat_cards.contains(src) || !m_combat_cards.contains(dest)) {
				Logger::log(Level::WARNING, "Invalid merge: Missing stack at ({}, {}) or ({}, {}).", src.first, src.second, dest.first, dest.second);
				return;
			}
			auto& src_stack = m_combat_cards[src];
			auto& dest_stack = m_combat_cards[dest];

			dest_stack.insert(dest_stack.end(),
				std::make_move_iterator(src_stack.begin()),
				std::make_move_iterator(src_stack.end()));
			src_stack.clear();
			m_combat_cards.erase(src);

			Logger::log(Level::INFO, "Merged stack from ({}, {}) to ({}, {}).", src.first, src.second, dest.first, dest.second);
			};
		if (forward) {
			for (size_t i = 0; i < coords.size() - 1; ++i) {
				merge(coords[i], coords[i + 1]);
			}
		}
		else {
			for (size_t i = coords.size() - 1; i > 0; --i) {
				merge(coords[i], coords[i - 1]);
			}
		}
		_reinitialise();
	}

	void Board::appendSpecialCard(const Coord& coord, CombatCard&& card) {
		/*if (!m_combat_cards.contains(coord)) {
			throw std::runtime_error("Stack does not exist at this position");
		}*/
		m_combat_cards[coord].push_back(std::move(card));

		_updateAvailableSpaces(coord);

		_setWinPosition(coord);
	}

	void Board::blockRow(uint16_t row, color::ColorType owner) {
		m_blocked_row = row;
		m_blocked_row_owner = owner;
	}

	void Board::blockColumn(uint16_t column, color::ColorType owner) {
		m_blocked_column = column;
		m_blocked_row_owner = owner;
	}

	void Board::clearBlockedRow() {
		m_blocked_row.reset();
	}

	void Board::clearBlockedColumn() {
		m_blocked_column.reset();
	}
	void Board::setLastPlayedCard(const CombatCard& card, const Coord& position) {
		m_last_card_type = card.getType();
		m_last_player_color = card.getColor();
		m_last_card_position = position;
		m_has_last_card = true;
	}

	void Board::incrementCardValue(const Coord& coord) {
		if (!m_combat_cards.contains(coord)) return;

		auto& stack = m_combat_cards[coord];
		if (stack.empty()) return;

		m_incremented_card = std::make_pair(coord, stack.back().getType());

		CombatCardType currentType = stack.back().getType();
		CombatCardType newType;

		switch (currentType) {
		case CombatCardType::ONE:
			newType = CombatCardType::TWO;
			break;
		case CombatCardType::TWO:
			newType = CombatCardType::THREE;
			break;
		case CombatCardType::THREE:
			newType = CombatCardType::FOUR;
			break;
		default:
			return;
		}

		color::ColorType cardColor = stack.back().getColor();
		stack.back() = CombatCard(newType, cardColor);

		Logger::log(Level::INFO, "Card at ({}, {}) incremented from {} to {}",
			coord.first, coord.second,
			combatCardToChar(currentType),
			combatCardToChar(newType));
	}

	void Board::resetIncrementCardValue() {
		if (!m_incremented_card.has_value()) return;

		const auto& [coord, originalType] = *m_incremented_card;
		if (!m_combat_cards.contains(coord)) {
			m_incremented_card.reset();
			return;
		}

		auto& stack = m_combat_cards[coord];
		if (stack.empty()) {
			m_incremented_card.reset();
			return;
		}

		color::ColorType cardColor = stack.back().getColor();
		stack.back() = CombatCard(originalType, cardColor);

		Logger::log(Level::INFO, "Card at ({}, {}) restored to original value {}",
			coord.first, coord.second,
			combatCardToChar(originalType));

		m_incremented_card.reset();
	}

	void Board::decrementCard(const Coord& coord) {
		if (!m_combat_cards.contains(coord)) return;

		auto& stack = m_combat_cards[coord];
		if (stack.empty()) return;

		m_decremented_card = std::make_pair(coord, stack.back().getType());

		CombatCardType currentType = stack.back().getType();
		CombatCardType newType;

		switch (currentType) {
		case CombatCardType::TWO:
			newType = CombatCardType::ONE;
			break;
		case CombatCardType::THREE:
			newType = CombatCardType::TWO;
			break;
		case CombatCardType::FOUR:
			newType = CombatCardType::THREE;
			break;
		default:
			return;
		}

		color::ColorType cardColor = stack.back().getColor();
		stack.back() = CombatCard(newType, cardColor);

		Logger::log(Level::INFO, "Opponent's card at ({}, {}) decreased from {} to {}",
			coord.first, coord.second,
			combatCardToChar(currentType),
			combatCardToChar(newType));
	}

	void Board::resetDecrementedCard() {
		if (!m_decremented_card.has_value()) return;

		const auto& [coord, originalType] = *m_decremented_card;
		if (!m_combat_cards.contains(coord)) {
			m_decremented_card.reset();
			return;
		}

		auto& stack = m_combat_cards[coord];
		if (stack.empty()) {
			m_decremented_card.reset();
			return;
		}

		color::ColorType cardColor = stack.back().getColor();
		stack.back() = CombatCard(originalType, cardColor);

		Logger::log(Level::INFO, "Opponent's card at ({}, {}) restored to original value {}",
			coord.first, coord.second,
			combatCardToChar(originalType));

		m_decremented_card.reset();
	}

	bool Board::hasLastPlayedCard() const {
		return m_has_last_card;
	}

	CombatCardType Board::getLastCardType() const {
		return m_last_card_type;
	}

	color::ColorType Board::getLastPlayerColor() const {
		return m_last_player_color;
	}

	Coord Board::getLastCardPosition() const {
		return m_last_card_position;
	}




	/*void Board::moveRow(uint16_t from_y, uint16_t to_y) {
		if (from_y == to_y) {
			throw std::runtime_error("from and to are the same");
		}

		std::vector<Coord> from_coords;

		for (auto it = m_combat_cards.begin(); it != m_combat_cards.end(); ) {
			if (it->first.second == from_y) {
				from_coords.push_back(it->first);
				it = m_combat_cards.erase(it);
			}
			else {
				++it;
			}
		}

		for (const auto& [x, y] : from_coords) {
			Coord new_coord{ to_y, y };

			m_combat_cards[new_coord] = std::move(
				m_combat_cards.extract({ x, y }).mapped()
			);
		}

		_reinitialise();

		Logger::log(Level::INFO, "Moved column {} to {}", from_y, to_y);
	}*/

	void Board::moveRow(uint16_t y, uint16_t newY) {
		if (y == newY) {
			Logger::log(Level::WARNING, "Source and destination rows are the same.");
			return;
		}
		std::vector<std::pair<Coord, std::vector<CombatCard>>> rowData;
		for (auto it = m_combat_cards.begin(); it != m_combat_cards.end();) {
			if (it->first.second == y) {
				rowData.emplace_back(it->first, std::move(it->second));
				it = m_combat_cards.erase(it);
			}
			else {
				++it;
			}
		}
		for (auto& pair : rowData) {
			Coord newCoord = { pair.first.first, newY };
			m_combat_cards[newCoord] = std::move(pair.second);
		}
		_reinitialise();
		Logger::log(Level::INFO, "Moved row {} to {}", y, newY);
	}

	void Board::moveColumn(uint16_t x, uint16_t newX) {
		if (x == newX) {
			Logger::log(Level::WARNING, "Source and destination columns are the same.");
			return;
		}
		std::vector<std::pair<Coord, std::vector<CombatCard>>> columnData;
		for (auto it = m_combat_cards.begin(); it != m_combat_cards.end(); ) {
			if (it->first.first == x) {
				columnData.emplace_back(it->first, std::move(it->second));
				it = m_combat_cards.erase(it);
			}
			else {
				++it;
			}
		}
		for (auto& pair : columnData) {
			Coord newCoord = { newX, pair.first.second };
			m_combat_cards[newCoord] = std::move(pair.second);
		}
		_reinitialise();
		Logger::log(Level::INFO, "Moved column {} to {}", x, newX);
	}

	/*void Board::moveColumn(uint16_t from_x, uint16_t to_x) {
		if (from_x == to_x) {
			throw std::runtime_error("froma nd to are the same");
		}

		std::vector<Coord> from_coords;

		for (auto it = m_combat_cards.begin(); it != m_combat_cards.end(); ) {
			if (it->first.first == from_x) {
				from_coords.push_back(it->first);
				it = m_combat_cards.erase(it);
			}
			else {
				++it;
			}
		}

		for (const auto& [x, y] : from_coords) {
			Coord new_coord{ to_x, y };

			m_combat_cards[new_coord] = std::move(
				m_combat_cards.extract({ x, y }).mapped()
			);
		}

		_reinitialise();

		Logger::log(Level::INFO, "Moved column {} to {}", from_x, to_x);
	}*/

	void Board::swapStacks(const Coord& from_coord, const Coord& to_coord) {
		auto from_it = m_combat_cards.find(from_coord);
		if (from_it == m_combat_cards.end()) {
			Logger::log(Level::ERROR, "No stack at ({},{})\n", from_coord.first, from_coord.second);
			return;
		}

		auto to_it = m_combat_cards.find(to_coord);
		if (to_it == m_combat_cards.end()) {
			Logger::log(Level::ERROR, "No stack at ({},{})\n", to_coord.first, to_coord.second);
			return;
		}

		auto& from_stack = from_it->second;
		auto& to_stack = to_it->second;

		std::vector<CombatCard> temp = std::move(from_stack);
		from_stack = std::move(to_stack);
		to_stack = std::move(temp);

		_reinitialise();

		Logger::log(Level::INFO, "Stacks swapped between ({}, {}) and ({}, {})",
			from_coord.first, from_coord.second, to_coord.first, to_coord.second);
	}

	CombatCard&& Board::popTopCardAt(const Coord& coord) {
		if (m_combat_cards.contains(coord) == false) {
			throw std::runtime_error("card does not exist");
		}

		if (m_combat_cards[coord].size() == 1 && this->isValidRemoveStack(coord) == false) {
			throw std::runtime_error("breaks adjacency");
		}
		if (m_incremented_card.has_value() && m_incremented_card->first == coord) {
			resetIncrementCardValue();
		}
		if (m_decremented_card.has_value() && m_decremented_card->first == coord) {
			resetDecrementedCard();
		}

		CombatCard card = std::move(m_combat_cards[coord].back());
		logger::Logger::log(logger::Level::INFO, "{}", card.getID());
		m_combat_cards[coord].pop_back();

		if (m_combat_cards[coord].empty()) {
			m_combat_cards.erase(coord);

			_reinitialise();
		}

		Logger::log(Level::INFO, "popped card at ({}, {})", coord.first, coord.second);

		return std::move(card);
	}

	CombatCard&& Board::popCardFromStackAt(const Coord& coord, const CombatCard& card_to_remove) {
		if (m_combat_cards.contains(coord) == false) {
			throw std::runtime_error("not a valid pos");
		}

		auto& stack = m_combat_cards[coord];
		auto card_it = std::find(stack.begin(), stack.end(), card_to_remove);

		if (card_it == stack.end()) {
			throw std::runtime_error("card not found");
		}

		if (m_combat_cards[coord].size() == 1 && this->isValidRemoveStack(coord) == false) {
			throw std::runtime_error("not conex anymore");
		}

		CombatCard card = std::move(*card_it);

		stack.erase(card_it);

		if (stack.empty()) {
			m_combat_cards.erase(coord);

			_reinitialise();
		}

		Logger::log(Level::INFO, "Removed card from the stack at ({}, {})", coord.first, coord.second);

		return std::move(card);
	}

	//Board::Stack&& Board::popStack(const Coord& coord) {
	//	if (m_combat_cards.contains(coord) == false) {
	//		throw std::runtime_error("not a valid pos");
	//	}

	//	if (this->isValidRemoveStack(coord) == false) {
	//		throw std::runtime_error("not conex anymore");
	//	}

	//	Stack stack = std::move(m_combat_cards[coord]);
	//	m_combat_cards.erase(coord);

	//	_reinitialise();

	//	Logger::log(Level::INFO, "stack removed at ({}, {})", coord.first, coord.second);

	//	return std::move(stack);
	//}

	Board::Stack Board::popStack(const Coord& coord) {
		if (!m_combat_cards.contains(coord)) {
			throw std::runtime_error("not a valid pos");
		}

		if (!this->isValidRemoveStack(coord)) {
			throw std::runtime_error("not conex anymore");
		}
		if (isIncrementedCard(coord)) {
			resetIncrementCardValue();
		}
		if (isDecrementedCard(coord)) {
			resetDecrementedCard();
		}
		Stack stack = std::move(m_combat_cards[coord]);
		m_combat_cards.erase(coord);

		_reinitialise();

		Logger::log(Level::INFO, "stack removed at ({}, {})", coord.first, coord.second);

		return stack;
	}

	std::vector<Board::Stack> Board::popRow(uint16_t y) {
		std::vector<Stack> row_stacks;

		if (m_bounding_rect.corner1.second <= y && y <= m_bounding_rect.corner2.second) {
			for (auto it = m_combat_cards.begin(); it != m_combat_cards.end(); ) {
				if (it->first.second == y) {
					row_stacks.push_back(std::move(it->second));

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

		return row_stacks;
	}

	std::vector<Board::Stack> Board::popColumn(uint16_t x) {
		std::vector<Stack> col_stacks;

		if (m_bounding_rect.corner1.first <= x && x <= m_bounding_rect.corner2.first) {
			for (auto it = m_combat_cards.begin(); it != m_combat_cards.end(); ) {
				if (it->first.first == x) {
					col_stacks.push_back(std::move(it->second));

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

		return col_stacks;
	}

	void Board::_addUsedCard(CombatCard&& card) {
		if (card.getColor() == color::ColorType::RED) {
			m_player1.addUsedCard(std::move(card));
		}
		else {
			m_player2.addUsedCard(std::move(card));
		}
	}

	void Board::_addUsedStack(Stack&& stack) {
		for (; stack.empty() == false; stack.pop_back()) {
			_addUsedCard(std::move(stack.back()));
		}
	}

	void Board::removeTopCardAt(const Coord& coord) {
		CombatCard card = this->popTopCardAt(coord);

		_addUsedCard(std::move(card));
	}

	void Board::removeCardFromStackAt(const Coord& coord, const CombatCard& card_to_remove) {
		CombatCard card = this->popCardFromStackAt(coord, card_to_remove);

		_addUsedCard(std::move(card));
	}

	void Board::removeStack(const Coord& coord) {
		Stack stack = this->popStack(coord);

		_addUsedStack(std::move(stack));
	}

	void Board::removeRow(uint16_t y) {
		if (!isValidRemoveRow(y)) {
			Logger::log(Level::ERROR, "You can't remove that row");
			return;
		}
		std::vector<Stack> row_stacks = std::move(popRow(y));

		for (; row_stacks.empty() == false; row_stacks.pop_back()) {
			_addUsedStack(std::move(row_stacks.back()));
		}
	}

	void Board::removeColumn(uint16_t x) {
		if (!isValidRemoveColumn(x)) {
			Logger::log(Level::ERROR, "You can't remove that column");
			return;
		}
		std::vector<Stack> col_stacks = std::move(popColumn(x));

		for (; col_stacks.empty() == false; col_stacks.pop_back()) {
			_addUsedStack(std::move(col_stacks.back()));
		}
	}

	const std::unordered_set<Coord, utils::CoordFunctor>& Board::availableSpaces() const {
		return m_available_spaces;
	}

	const std::unordered_map<Coord, std::vector<CombatCard>, utils::CoordFunctor>& Board::getCombatCards() const {
		return m_combat_cards;
	}

	//-----------------------------------------------------CHECKERS-----------------------------------------------------


	bool Board::isValidPlaceCard(const Coord& coord, const CombatCard& card) {


		if (m_combat_cards.contains(coord)) {
			if (card.getType() == CombatCardType::ETER) {
				Logger::log(Level::WARNING, "This card must be played on an empty space");
				return false;
			}


			if (this->getTopCard(coord)->get() >= card &&
				this->getTopCard(coord)->get().getType() != CombatCardType::BORDER) {

				Logger::log(Level::WARNING, "Card too small");
				return false;
			}
		}
		else if (m_available_spaces.contains(coord) == false) {
			Logger::log(Level::WARNING, "not available place");
			return false;
		}

		return true;
	}

	bool Board::isValidPlaceCard(const InputHandler& input) {
		if (m_combat_cards.contains(input.coord)) {
			if (input.card_type == CombatCardType::ETER) {
				return false;
			}

			if (this->getTopCard(input.coord)->get().isIllusion() && input.event_type == EventType::PlaceIllusion) {
				return false;
			}

			if (this->getTopCard(input.coord)->get().isIllusion() && this->getTopCard(input.coord)->get().getColor() != input.color) {
				return true;
			}

			if (this->getTopCard(input.coord)->get().isIllusion() && this->getTopCard(input.coord)->get().getColor() == input.color) {
				return false;
			}

			if (this->getTopCard(input.coord)->get().getType() >= input.card_type) {
				return false;
			}
		}
		else if (m_available_spaces.contains(input.coord) == false) {
			return false;
		}

		return true;
	}

	bool Board::isValidRemoveStack(const Coord& remove_coord) const {
		if (m_combat_cards.contains(remove_coord) == false) {
			Logger::log(Level::WARNING, "not available place to remove");
			return false;
		}

		std::unordered_set<Coord, utils::CoordFunctor> new_config{
			std::ranges::begin(m_combat_cards | std::views::keys),
			std::ranges::end(m_combat_cards | std::views::keys)
		};

		new_config.erase(remove_coord);

		return _isConex(new_config);
	}

	bool Board::isValidRemoveRow(const uint16_t row_index) const
	{
		if (row_index < m_bounding_rect.corner1.second || row_index > m_bounding_rect.corner2.second) {
			Logger::log(Level::WARNING, "Row {} is out of bounds.", row_index);
			return false;
		}

		std::unordered_set<Coord, utils::CoordFunctor> simulated_config{
			std::ranges::begin(m_combat_cards | std::views::keys),
			std::ranges::end(m_combat_cards | std::views::keys)
		};

		for (auto it = simulated_config.begin(); it != simulated_config.end(); ) {
			if (it->second == row_index) {
				it = simulated_config.erase(it);
			}
			else {
				++it;
			}
		}
		return _isConex(simulated_config);

	}

	bool Board::isValidRemoveColumn(const uint16_t col_index) const {

		if (col_index < m_bounding_rect.corner1.first || col_index > m_bounding_rect.corner2.first) {
			Logger::log(Level::WARNING, "Column {} is out of bounds.", col_index);
			return false;
		}
		std::unordered_set<Coord, utils::CoordFunctor> simulated_config{
			std::ranges::begin(m_combat_cards | std::views::keys),
			std::ranges::end(m_combat_cards | std::views::keys)
		};
		for (auto it = simulated_config.begin(); it != simulated_config.end(); ) {
			if (it->first == col_index) {
				it = simulated_config.erase(it);
			}
			else {
				++it;
			}
		}
		return _isConex(simulated_config);
	}


	bool Board::isValidMoveStack(const Coord& from_coord, const Coord& to_coord) const {
		if (m_combat_cards.contains(from_coord) == false) {
			Logger::log(Level::WARNING, "not available stack to move");
			return false;
		}

		if (m_available_spaces.contains(to_coord) == false) {
			Logger::log(Level::WARNING, "not available place to move stack");
			return false;
		}

		if (from_coord == to_coord) {
			Logger::log(Level::WARNING, "choose a valid place");
			return false;
		}

		std::unordered_set<Coord, utils::CoordFunctor> new_config{
			std::ranges::begin(m_combat_cards | std::views::keys),
			std::ranges::end(m_combat_cards | std::views::keys)
		};

		new_config.erase(from_coord);
		new_config.insert(to_coord);

		return _isConex(new_config);
	}

	bool Board::_isConex(const std::unordered_set<Coord, utils::CoordFunctor>& board_coords) const { // bfs 
		if (board_coords.size() == 0 ||
			board_coords.size() == 1 // ||
			//board_coords.size() == 2
			) {
			return true;
		}

		std::unordered_set<Coord, utils::CoordFunctor> visited;
		std::queue<Coord> queue;

		Coord first_coord{ *board_coords.begin() };
		queue.push(first_coord);
		visited.insert(first_coord);


		for (; queue.empty() == false; queue.pop()) {
			Coord curr = queue.front();

			for (const auto& [x, y] : Config::getInstance().getOffsets()) {
				Coord adjacent_coord{ curr.first + x, curr.second + y };

				bool not_visited = visited.contains(adjacent_coord) == false;
				bool is_on_board = board_coords.contains(adjacent_coord);

				if (not_visited && is_on_board) {
					visited.insert(adjacent_coord);
					queue.push(adjacent_coord);
				}
			}
		}

		return visited.size() == board_coords.size(); // conex
	}

	bool Board::isValidMoveStacks(const std::vector<std::pair<Coord, Coord>>& moves) const {

		std::unordered_set<Coord, utils::CoordFunctor> new_config{
			std::ranges::begin(m_combat_cards | std::views::keys),
			std::ranges::end(m_combat_cards | std::views::keys)
		};

		bool is_first_stack = true;
		for (const auto& [from, to] : moves) {
			if (!new_config.contains(from)) {
				Logger::log(Level::WARNING, "Stack does not exist at source coordinate ({}, {}).", from.first, from.second);
				return false;
			}
			if (new_config.contains(to)) {
				Logger::log(Level::WARNING, "Destination coordinate ({}, {}) is not empty.", to.first, to.second);
				return false;
			}
			if (is_first_stack && !m_available_spaces.contains(to)) {
				Logger::log(Level::WARNING, "Destination coordinate ({}, {}) is invalid.", to.first, to.second);
				return false;
			}
			new_config.erase(from);
			new_config.insert(to);
			is_first_stack = false;
		}
		return _isConex(new_config);
	}

	bool Board::isValidMergeStacks(const std::vector<Coord>& coords, const Coord& final_coord) const {
		if (coords.empty()) {
			Logger::log(Level::WARNING, "No source stacks provided for merge.");
			return false;
		}
		if (std::find(coords.begin(), coords.end(), final_coord) == coords.end()) {
			Logger::log(Level::WARNING, "Final destination ({}, {}) is not in the source coordinates.", final_coord.first, final_coord.second);
			return false;
		}
		if (!m_combat_cards.contains(final_coord)) {
			Logger::log(Level::WARNING, "Final destination ({}, {}) is not valid.", final_coord.first, final_coord.second);
			return false;
		}
		std::unordered_set<Coord, utils::CoordFunctor> simulated_config{
			std::ranges::begin(m_combat_cards | std::views::keys),
			std::ranges::end(m_combat_cards | std::views::keys)
		};
		for (const auto& coord : coords) {
			if (coord != final_coord) {
				if (!simulated_config.contains(coord)) {
					Logger::log(Level::WARNING, "Stack does not exist at ({}, {}).", coord.first, coord.second);
					return false;
				}
				simulated_config.erase(coord);
			}
		}
		return _isConex(simulated_config);
	}

	bool Board::isRowBlocked(uint16_t row, color::ColorType player_color) const {
		return m_blocked_row.has_value() &&
			*m_blocked_row == row &&
			m_blocked_row_owner != player_color;
	}

	bool Board::isColumnBlocked(uint16_t column, color::ColorType player_color) const
	{
		return m_blocked_column.has_value() &&
			*m_blocked_column == column &&
			m_blocked_row_owner != player_color;
	}

	bool Board::isIncrementedCard(const Coord& coord) const {
		return m_incremented_card.has_value() && m_incremented_card->first == coord;
	}

	bool Board::isDecrementedCard(const Coord& coord) const {
		return m_decremented_card.has_value() && m_decremented_card->first == coord;
	}

	///------------------------------Setter Getter------------------------------------

	uint16_t Board::size() const {
		return m_size;
	}

	bool Board::isFixed() const {
		return m_bounding_rect.isFixed();
	}

	bool Board::fixedWidth() const {
		return m_bounding_rect.fixed_width;
	}

	bool Board::fixedHeight() const {
		return m_bounding_rect.fixed_height;
	}

	bool Board::hasStack(const Coord& coord) const {
		return m_combat_cards.contains(coord);
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

	bool Board::isFixedColumn(int16_t x) const
	{

		int16_t count_elements = 0;
		for (const auto& [coord, stack] : m_combat_cards) {
			if (coord.first == x) {
				count_elements++;
			}

		}
		return count_elements == m_size;
	}

	bool Board::isFixedRow(int16_t y) const
	{

		int16_t count_elements = 0;
		for (const auto& [coord, stack] : m_combat_cards) {
			if (coord.second == y) {
				count_elements++;
			}
		}
		return count_elements == m_size;
	}

	std::unordered_set<int16_t> Board::getFixedRows()const {

		std::unordered_set<int16_t> fixed_rows;
		std::unordered_set<int16_t> rows;

		for (const auto& [coord, stack] : m_combat_cards) {
			rows.insert(coord.second);
		}
		for (const auto& y : rows) {
			if (isFixedRow(y)) {
				fixed_rows.insert(y);
			}
		}
		return fixed_rows;

	}


	std::unordered_set<int16_t> Board::getFixedColumns() const {

		std::unordered_set<int16_t> fixed_columns;
		std::unordered_set<int16_t> columns;

		for (const auto& [coord, stack] : m_combat_cards) {
			columns.insert(coord.first);
		}
		for (const auto& x : columns) {
			if (isFixedColumn(x)) {
				fixed_columns.insert(x);
			}
		}
		return  fixed_columns;
	}

	std::unordered_set<int16_t> Board::getUnfixedRows() const
	{
		std::unordered_set<int16_t> unfixed_rows;
		std::unordered_set<int16_t> rows;

		for (const auto& [coord, stack] : m_combat_cards) {
			rows.insert(coord.second);
		}
		for (const auto& y : rows) {
			if (!isFixedRow(y)) {
				unfixed_rows.insert(y);
			}
		}
		return unfixed_rows;
	}

	std::unordered_set<int16_t> Board::getUnfixedColumns() const
	{
		std::unordered_set<int16_t> unfixed_columns;
		std::unordered_set<int16_t> columns;

		for (const auto& [coord, stack] : m_combat_cards) {
			columns.insert(coord.first);
		}
		for (const auto& x : columns) {
			if (!isFixedColumn(x)) {
				unfixed_columns.insert(x);
			}
		}
		return  unfixed_columns;
	}

	Coord Board::getRightMostOnRow(int16_t y) const {
		Coord rightmost = { 0, y };

		for (const auto& [coord, stack] : m_combat_cards) {
			if (coord.second == y && coord.first > rightmost.first) {
				rightmost = coord;
			}
		}

		return rightmost;
	}
	Coord Board::getLeftMostOnRow(int16_t y) const {
		Coord leftmost = { std::numeric_limits<int16_t>::max(), y };

		for (const auto& [coord, stack] : m_combat_cards) {
			if (coord.second == y && coord.first < leftmost.first) {
				leftmost = coord;
			}
		}

		return leftmost;
	}
	Coord Board::getTopMostOnColumn(int16_t x) const {
		Coord topmost = { x, std::numeric_limits<int16_t>::max() };

		for (const auto& [coord, stack] : m_combat_cards) {
			if (coord.first == x && coord.second < topmost.second) {
				topmost = coord;
			}
		}

		return topmost;
	}
	Coord Board::getBottomMostOnColumn(int16_t x) const {
		Coord bottommost = { x, 0 };

		for (const auto& [coord, stack] : m_combat_cards) {
			if (coord.first == x && coord.second > bottommost.second) {
				bottommost = coord;
			}
		}

		return bottommost;
	}

	std::vector<Coord> Board::getCoordsOnRow(int16_t row_index) const {
		std::vector<Coord> coords;
		for (const auto& [coord, stack] : m_combat_cards) {
			if (coord.second == row_index) {
				coords.push_back(coord);
			}
		}
		std::sort(coords.begin(), coords.end(), [](const Coord& a, const Coord& b) { ///
			return a.first < b.first;
			});
		return coords;
	}

	std::vector<Coord> Board::getCoordsOnColumn(int16_t col_index) const {
		std::vector<Coord> coords;
		for (const auto& [coord, stack] : m_combat_cards) {
			if (coord.first == col_index) {
				coords.push_back(coord);
			}
		}
		std::sort(coords.begin(), coords.end(), [](const Coord& a, const Coord& b) { /// 
			return a.second < b.second;
			});
		return coords;
	}


	uint16_t Board::getSize() const {
		return m_size;
	}

	const std::vector<CombatCard>& Board::operator[](const Coord& coord) const {
		return m_combat_cards.at(coord);
	}

	std::vector<CombatCard>& Board::operator[](const Coord& coord) {
		return m_combat_cards.at(coord);
	}

	std::optional<CombatCardRef> Board::getTopCard(const Coord& pos) {
		if (m_combat_cards.contains(pos)) {
			return std::ref(m_combat_cards[pos].back());
		}
		return std::nullopt;
	}

	Coord Board::getLeftCorner() const {
		return m_bounding_rect.corner1;
	}

	std::pair<Coord, Coord> Board::getBoudingRect() const {
		return { m_bounding_rect.corner1, m_bounding_rect.corner2 };
	}

	std::pair<uint16_t, uint16_t> Board::getBoundingRectSize() const {
		return {
			(m_bounding_rect.corner2.first - m_bounding_rect.corner1.first + 2) / 2,
			m_bounding_rect.corner2.second - m_bounding_rect.corner1.second + 1
		};
	}

	std::optional<Coord> Board::getWinCoord() const {
		return m_win_pos;
	}

	void Board::shift(const Coord& offset) {
		std::unordered_map<Coord, Stack, utils::CoordFunctor> temp;

		for (auto& [coord, stack] : m_combat_cards) {
			Coord new_coord {
				coord.first + offset.first,
				coord.second + offset.second
			};
			
			temp.emplace(new_coord, std::move(stack));
		}

		m_combat_cards = std::move(temp);
	}

	///----------------------------Private Methods--------------------------------
	void Board::_setWinPosition(const Coord& coord) {
		std::unordered_map<uint16_t, int16_t> x_count, y_count;
		x_count.reserve(m_size);
		y_count.reserve(m_size);
		int16_t diag1{ 0 }, diag2{ 0 };

		for (const auto& [coord, stack] : m_combat_cards) {
			auto [x, y] { coord };
			color::ColorType color{ stack.back().getColor() };
			int16_t incr{ (color == color::ColorType::RED) ? -1 : 1 };

			x_count[x] += incr;
			y_count[y] += incr;

			if (this->isFixed()) {
				Coord mapped{ utils::mapCoordToMatrix(this->getLeftCorner(), coord) };

				if (mapped.first == mapped.second) {
					diag1 += incr;
				}

				if (mapped.first + mapped.second == m_size - 1) {
					diag2 += incr;
				}
			}
		}

		const auto& [x, y] = coord;

		bool won_on_lines = abs(diag1) == m_size || abs(diag2) == m_size;
		bool won_on_diags = abs(y_count[y]) == m_size || abs(x_count[x]) == m_size;

		if (won_on_lines || won_on_diags) {
			m_win_pos.emplace(coord);
		}
	}


	void Board::_updateAvailableSpaces(const Coord& coord) {
		m_available_spaces.erase(coord);

		for (const auto& offset : Config::getInstance().getOffsets()) {
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

	/// ---------------------------------------------------CONSOLE-RENDER--------------------------------------------------


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
				card_refs.emplace_back(std::ref(card));
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
					print_pos.first + uint16_t(pos.size()) + j,
					print_pos.second + i
				);
				j += 2;
			}

			i++;
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
		corner1{ Config::getInstance().getStartPoint() }, corner2{ Config::getInstance().getStartPoint() },
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

		uint16_t offset_x = Config::getInstance().getCardSpacingX();
		uint16_t offset_y = Config::getInstance().getCardSpacingY();

		fixed_width = ((corner2.first - corner1.first) / offset_x + 1 == size) ? true : false;
		fixed_height = ((corner2.second - corner1.second) / offset_y + 1 == size) ? true : false;
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

	Board::const_iterator Board::begin() const {
		return m_combat_cards.begin();
	}

	Board::const_iterator Board::end() const {
		return m_combat_cards.end();
	}

}