#include "PowerCard.h"

namespace base {
	PowerCard::PowerCard(Board& board, Player& red, Player& blue) :
		m_board{board},
		m_player_red{red},
		m_player_blue{blue} {

	}

	std::string_view PowerCard::getAbilityName()const {
		return typeToStrings(m_ability);
	}
}