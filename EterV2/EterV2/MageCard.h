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

		virtual void apply(Board& board, Player& player) = 0;
		
	protected:
		MageType m_type;
		MageTypeAbility m_ability;
	};
	
	/*
	inline std::ostream& operator<<(std::ostream& out, const MageCard& card) {
		out << "Mage Type: " << card.getTypeName() << "\n"
			<< "Front Ability: " << card.getFrontAbilityName() << "\n"
			<< "Back Ability: " << card.getBackAbilityName() << "\n";
		return out;
	}*/
}