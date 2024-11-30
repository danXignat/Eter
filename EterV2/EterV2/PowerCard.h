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
		PowerCard() = default;
		virtual ~PowerCard() = default;

		std::string_view getAbilityName() const;

		virtual void apply(Board& board, Player& player) = 0;

	protected:

		PowerCardType m_ability;
	};
}