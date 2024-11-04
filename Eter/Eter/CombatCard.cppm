export module CombatCard;

import CombatCardType;

namespace base {
	export class CombatCard {
	private:
		CombatCardType m_combat_card_type;

	public:
		CombatCard(CombatCardType type);
		CombatCardType getType();

		bool operator<(const CombatCard& other) const;
		bool operator==(const CombatCard& other)const;
	};
}