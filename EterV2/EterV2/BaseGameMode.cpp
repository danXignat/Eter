#include "BaseGameMode.h"

#include "logger.h"

namespace base {
	BaseGameMode::BaseGameMode(GameSizeType game_size, const std::pair<std::string, std::string>& player_names) :
		m_win_manager{m_board},
		m_player_red{ player_names.first, color::ColorType::RED, game_size },
		m_player_blue{ player_names.second, color::ColorType::BLUE, game_size },
		m_board{ game_size, m_player_red, m_player_blue },
		m_curr_player{m_player_red} {

	}

	void BaseGameMode::_switchPlayer() {
		if (m_curr_player.get().getColor() == color::ColorType::RED) {
			m_curr_player = m_player_blue;
		}
		else {
			m_curr_player = m_player_red;
		}
	}

	bool BaseGameMode::_handleEvent(const InputHandler& input) {
		if (input.event_type == GameEventType::BoardCard) {
			return _handleBoardEvent(input);
		}
		else if (input.event_type == GameEventType::SpecialCard) {
			return _handleSpecialEvent(input);
		}
	}

	bool BaseGameMode::_handleBoardEvent(const InputHandler& input) {
		CombatCardType card_type = input.card_type;
		Coord coord = input.coord;
		const CombatCard& card_view = m_curr_player.get().viewCard(card_type);

		auto board_card = m_board.getTopCard(coord);
		bool is_illusion = board_card && board_card->get().isIllusion();
		if (is_illusion && IllusionService::hasIllusionWon(board_card.value(), card_view)) {
			logger::Logger::log(logger::Level::INFO, "illusion has won");
			return true;
		}

		if (m_board.isValidPlaceCard(coord, card_view)) {
			CombatCard card = m_curr_player.get().getCard(card_type);

			m_board.appendMove(coord, std::move(card));

			m_win_manager.addCard(coord);

			return true;
		}

		return false;
	}

	



}