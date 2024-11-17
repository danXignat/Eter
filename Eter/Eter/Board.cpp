module Board;

import <iostream>;
import <memory>;
import <vector>;
import <stack>;
import <unordered_map>;
import <unordered_set>;

#include <array>
#include <limits.h>

import Logger;
import Player;
import CombatCard;
import utils;

using namespace logger;

namespace base {
	//----------------------------Constructors--------------------------------

	Board::Board(uint16_t size) :
		m_size{ size },
		m_bounding_rect{ size },
		m_available_spaces{ Coord{10, 5} } {

	}

	//----------------------------Public-Methods--------------------------------

	void Board::appendMove(Coord coord, CardPtr&& card_ptr) {
		if (_isValidPos(coord)) {
			m_bounding_rect.add(coord);

			m_combat_cards[coord].emplace_back(std::move(card_ptr));
			Logger::log(Level::INFO, "card({}, {})", coord.first, coord.second);

			_updateAvailableSpaces(coord);
		}
		else if (m_combat_cards[coord].back()->isIllusion()) {
			bool win = *m_combat_cards[coord].back() < *card_ptr;
			m_combat_cards[coord].back()->revealIllusion();

			if (win) {
				m_combat_cards[coord].emplace_back(std::move(card_ptr));
			}
			else {
				card_ptr.reset();
			}
		}
		else if (_isValidMove(coord, *card_ptr)) {
			m_combat_cards[coord].emplace_back(std::move(card_ptr));
		}
		else {
			Logger::log(Level::WARNING, "position not valid");
		}
	}

	void Board::renderBoard() const {
		for (const auto& pair : m_combat_cards) {
			utils::printAtCoordinate(*pair.second.back(), pair.first.first, pair.first.second);
		}

		for (const auto& pair : m_available_spaces) {
			utils::printAtCoordinate("*", pair.first, pair.second);
		}

	}

	Coord Board::getCountDiag() {
		auto [x, y] = m_bounding_rect.corner1;

		Coord diags_count{0, 0};

		for (uint16_t i = 0; i < m_size; i++) {
			Coord coord{ x + i, y + i };
			if (m_combat_cards.contains(coord)) {
				teams::Team team = m_combat_cards[coord].back()->getTeam();

				diags_count.first += (team == teams::Team::BLUE) ? 1 : -1;
			}
		}
		return diags_count;
	}

	//------------------------------Setter Getter------------------------------------

	bool Board::isFixed() const {
		return m_bounding_rect.isFixed();
	}


	uint16_t Board::getSize() const {
		return m_size;
	}

	//----------------------------Private Methods--------------------------------
	bool Board::_isValidPos(Coord coord) const {
		return m_available_spaces.contains(coord);
	}

	bool Board::_isValidMove(Coord coord, const CombatCard& card) {
		bool is_bottom_card = m_combat_cards.contains(coord);
		bool is_bigger = (*m_combat_cards[coord].back() < card);

		return is_bottom_card && is_bigger;
	}

	const std::unordered_map<Coord, std::vector<std::unique_ptr<CombatCard>>, Board::CoordFunctor>& Board::getCombatCards()const {
		return m_combat_cards;
	}

	std::unordered_map<Coord, std::vector<std::unique_ptr<CombatCard>>, Board::CoordFunctor>& Board::getCombatCards(){
		return m_combat_cards;
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

	size_t Board::CoordFunctor::operator()(const Coord& coord) const {
		std::hash<uint16_t> hasher;

		return hasher(coord.first) ^ hasher(coord.second << 1);
	}

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

	bool Board::BoundingRect::isFixed() const {
		return fixed_width && fixed_height;
	}

	void Board::squallCard(Player& currentPlayer) {
		Player& opponent = (m_player1.getTeam() == currentPlayer.getTeam()) ? m_player2 : m_player1;

		for (auto& pair : m_combat_cards) {
			const auto& coord = pair.first;
			auto& cards = pair.second;

			if (cards.empty()) {
				continue;
			}
			for (auto& card : cards) {

				if (card->getTeam() != currentPlayer.getTeam()) { 

					opponent.addCard(std::move(card));
					cards.erase(std::remove(cards.begin(), cards.end(), card), cards.end());

					return; 
				}
			}
		}

		Logger::log(Level::WARNING, "No visible card to return.");
	}

	}