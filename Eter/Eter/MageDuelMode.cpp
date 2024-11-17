//import MageDuelMode;
//import MageCardType;
//import MageCardTypeAbility;
//import<random>;
//#include <algorithm>
//#include <stdexcept>
//import Logger;
//
//using namespace logger;
//namespace base {
//
//	MageDuelMode::MageDuelMode(std::string Player1, std::string Player2) :
//		TrainingMode(Player1, Player2)
//
//	{
//		m_board = Board{ 4 };
//		m_win_manager = WinManager{ 4 };
//		initializeMageCards();
//		assignRandomCardToPlayers();
//	}
//
//	void MageDuelMode::initializeMageCards() {
//
//		using enum MageCardTypeAbility;
//		using enum MageCardType;
//		m_mageCardsPool.emplace_back(std::make_unique<MageCard>(Fire, Burn, BurnRowOrColumn));
//		m_mageCardsPool.emplace_back(std::make_unique<MageCard>(Earth, Bury, Hole));
//		m_mageCardsPool.emplace_back(std::make_unique<MageCard>(Air, BlowAway, BlowEter));
//		m_mageCardsPool.emplace_back(std::make_unique<MageCard>(Water, Boat, BoatRowOrColumn));
//
//	}
//
//	void MageDuelMode::assignRandomCardToPlayers() {
//		std::random_device randomDev;
//		std::mt19937 gen(randomDev());
//		std::shuffle(m_mageCardsPool.begin(), m_mageCardsPool.end(), gen);
//		if (!m_mageCardsPool.empty()) {
//			m_player1.setMageCard(std::move(m_mageCardsPool.back()));
//			m_mageCardsPool.pop_back();
//		}
//		if (!m_mageCardsPool.empty()) {
//			m_player2.setMageCard(std::move(m_mageCardsPool.back()));
//			m_mageCardsPool.pop_back();
//		}
//	}
//
//	void MageDuelMode::removeOpponentCard(const Coord& coord, Player& player) {
//
//		auto& combat_cards = m_board.getCombatCards();
//
//		auto it = combat_cards.find(coord);
//		if (it != combat_cards.end() && it->second.size() + 1) {
//			auto& card_stack = it->second;
//			if (card_stack.back()->getTeam() != player.getTeam()) {
//				auto& second_to_last_card = *(card_stack.rbegin() + 1);
//				if (second_to_last_card->getTeam() == player.getTeam()) {
//					card_stack.pop_back();
//					Logger::log(Level::INFO, "Removed opponent's card covering your card at ({}, {})", coord.first, coord.second);
//
//					if (card_stack.empty()) {
//						combat_cards.erase(it);
//						Logger::log(Level::INFO, "Removed last card from the stack at ({},{})", coord.first, coord.second);
//
//					}
//					return;
//				}
//			}
//			Logger::log(Level::WARNING, "Top card does not cover your card or is your card at ({}, {})", coord.first, coord.second);
//		}
//		else {
//			Logger::log(Level::WARNING, "No stack or only one card present at ({}, {})", coord.first, coord.second);
//		}
//	}
//
//
//	void MageDuelMode::removeRow(const Coord& start_coord, Player& player) {
//		int row_count = 0;
//		bool own_card_found = false;
//
//		auto& combat_cards = m_board.getCombatCards();
//		Coord current_coord = start_coord;
//
//		while (combat_cards.find(current_coord) != combat_cards.end()) {
//			auto& card_stack = combat_cards[current_coord];
//			if (!card_stack.empty()) {
//				if (card_stack.back()->getTeam() == player.getTeam()) {
//					own_card_found = true;
//				}
//				row_count++;
//			}
//			current_coord.first++;
//		}
//		if (row_count >= 3 && own_card_found) {
//			current_coord = start_coord;
//			for (int i = 0; i < row_count; i++) {
//				combat_cards.erase(current_coord);
//				current_coord.first++;
//			}
//			Logger::log(Level::INFO, "Removed entire row starting from ({}, {})", start_coord.first, start_coord.second);
//		}
//		else {
//			Logger::log(Level::WARNING, "Cannot remove row, conditions not met at ({}, {})", start_coord.first, start_coord.second);
//
//		}
//	}
//
//
//	void MageDuelMode::removeColumn(const Coord& start_coord, Player& player) {
//		int column_count = 0;
//		bool own_card_found = false;
//
//		auto& combat_cards = m_board.getCombatCards();
//		Coord current_coord = start_coord;
//
//		while (combat_cards.find(current_coord) != combat_cards.end()) {
//			auto& card_stack = combat_cards[current_coord];
//			if (!card_stack.empty()) {
//				if (card_stack.back()->getTeam() == player.getTeam()) {
//					own_card_found = true;
//				}
//				column_count++;
//			}
//			current_coord.second++;
//		}
//
//		if (column_count >= 3 && own_card_found) {
//			current_coord = start_coord;
//			for (int i = 0; i < column_count; i++) {
//				combat_cards.erase(current_coord);
//				current_coord.second++;
//			}
//			Logger::log(Level::INFO, "Removed entire column starting from ({}, {})", start_coord.first, start_coord.second);
//		}
//		else {
//			Logger::log(Level::WARNING, "Cannot remove column, conditions not met at ({}, {})", start_coord.first, start_coord.second);
//		}
//	}
//
//	void MageDuelMode::moveOpponentsStack(const Coord& fromCoord, const Coord& toCoord, Player& player) {
//		auto& combat_cards = m_board.getCombatCards();
//
//		auto fromIt = combat_cards.find(fromCoord);
//
//		if (fromIt != combat_cards.end() && !fromIt->second.empty()) {
//			if (fromIt->second.back()->getTeam() != player.getTeam()) {
//				if (combat_cards.find(toCoord) == combat_cards.end()) {
//					combat_cards[toCoord] = std::move(fromIt->second);
//					combat_cards.erase(fromIt);
//					Logger::log(Level::INFO, "Moved stack from ({}, {}) to ({}, {})", fromCoord.first, fromCoord.second, toCoord.first, toCoord.second);
//				}
//				else {
//					Logger::log(Level::WARNING, "Destination position ({}, {}) is not empty", toCoord.first, toCoord.second);
//				}
//			}
//			else {
//				Logger::log(Level::WARNING, "Top card at original position ({}, {}) is not an opponent's card", fromCoord.first, fromCoord.second);
//			}
//		}
//
//		else {
//			Logger::log(Level::WARNING, "No stack found at the original position ({}, {})", fromCoord.first, fromCoord.second);
//		}
//	}
//
//	void MageDuelMode::moveOwnStack(const Coord& fromCoord, const Coord& toCoord, Player& player) {
//		auto& combat_cards = m_board.getCombatCards();
//
//		auto fromIt = combat_cards.find(fromCoord);
//
//		if (fromIt != combat_cards.end() && !fromIt->second.empty()) {
//			if (fromIt->second.back()->getTeam() == player.getTeam()) {
//				if (combat_cards.find(toCoord) == combat_cards.end()) {
//					combat_cards[toCoord] = std::move(fromIt->second);
//					combat_cards.erase(fromIt);
//					Logger::log(Level::INFO, "Moved stack from ({}, {}) to ({}, {})", fromCoord.first, fromCoord.second, toCoord.first, toCoord.second);
//				}
//				else {
//					Logger::log(Level::WARNING, "Destination position ({}, {}) is not empty", toCoord.first, toCoord.second);
//				}
//			}
//			else {
//				Logger::log(Level::WARNING, "Top card at original position ({}, {}) is not an opponent's card", fromCoord.first, fromCoord.second);
//			}
//		}
//
//		else {
//			Logger::log(Level::WARNING, "No stack found at the original position ({}, {})", fromCoord.first, fromCoord.second);
//		}
//	}
//
//
//}