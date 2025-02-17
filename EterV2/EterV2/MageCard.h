#pragma once
#include<iostream>


#include "Board.h"
#include "Player.h"
#include "typedefs.h"

#include "MageTypeAbility.h"

namespace base {
	class MageCard {
	public:
		MageCard(Board& board, Player& player_red, Player& player_blue, color::ColorType color);
		virtual ~MageCard() = default;

		MageType getType() const;
		MageTypeAbility getMasterType() const;

		virtual bool apply() = 0;
		
	protected:
		MageType m_type;
		MageTypeAbility m_ability;
		color::ColorType m_color;

		Board& m_board;
		Player& m_player_red;
		Player& m_player_blue;
	};

}