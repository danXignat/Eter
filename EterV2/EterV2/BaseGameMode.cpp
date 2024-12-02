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
}