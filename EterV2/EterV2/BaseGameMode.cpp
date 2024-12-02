#include "BaseGameMode.h"

namespace base {
	BaseGameMode::BaseGameMode(GameSizeType game_size, const std::pair<std::string, std::string>& player_names) :
		m_board{ game_size },
		m_win_manager{m_board},
		m_player_red{ player_names.first, color::ColorType::RED, game_size },
		m_player_blue{ player_names.second, color::ColorType::BLUE, game_size },
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
		switch (input.event_type) {
		case GameEventType::BoardCard:
			return _handleBoardEvent(input);
			break;
		case GameEventType::SpecialCard:
			_handleSpecialEvent(input);
			break;
		default:
			break;
		}
	}

	bool BaseGameMode::_handleBoardEvent(const InputHandler& input) {
		CombatCardType card_type = input.card_type;
		Coord coord = input.coord;

		const CombatCard& card_view = m_curr_player.get().viewCard(card_type);

		if (m_board.isValidMove(coord, card_view)) {
			CombatCard card = m_curr_player.get().getCard(card_type);

			m_board.appendMove(coord, std::move(card));

			m_win_manager.addCard(coord);

			return true;
		}

		return false;
	}

	



}