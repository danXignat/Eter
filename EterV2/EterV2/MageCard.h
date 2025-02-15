#pragma once
#include<iostream>


#include "Board.h"
#include "Player.h"
#include "typedefs.h"

#include "MageTypeAbility.h"

namespace base {
	class MageCard {
	public:
		MageCard();
		virtual ~MageCard() = default;

		MageType getType() const;
		MageTypeAbility getMasterType() const;

		virtual bool apply(Board& board, Player& player) = 0;
		
	protected:
		MageType m_type;
		MageTypeAbility m_ability;
	};

}