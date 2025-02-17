#pragma once
#include<iostream>

#include"Board.h"
#include"Player.h"
#include"typedefs.h"
#include"PowerCardType.h"
#include "GameModeType.h"

namespace base {
	class PowerCard {
	public:
		PowerCard(Board& board, Player& red, Player& blue);
		virtual ~PowerCard() = default;

		std::string_view getAbilityName() const;

		virtual void apply() = 0;

	protected:
		Board& m_board;
		Player& m_player_red;
		Player& m_player_blue;

		PowerCardType m_ability;
	};
}