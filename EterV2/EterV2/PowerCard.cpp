#include "PowerCard.h"

namespace base {
	PowerCard::PowerCard(Board& board, Player& red, Player& blue) :
		m_board{board},
		m_player_red{red},
		m_player_blue{blue} {
		static uint16_t counter = 0;

		m_id = counter;

		counter++;
	}

	std::string_view PowerCard::getAbilityName()const {
		return typeToStrings(m_ability);
	}

	uint16_t PowerCard::getId() const {
		return m_id;
	}

	PowerCardType PowerCard::getType() const {
		return m_ability;
	}
}