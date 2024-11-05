export module CombatCard;

import <iostream>;

import CombatCardType;

namespace base {
	export class CombatCard {
	private:
		CombatCardType m_type;

	public:
		CombatCard(CombatCardType type);
		CombatCardType getType();

		bool operator<(const CombatCard& other) const;
		bool operator==(const CombatCard& other) const;

		friend std::ostream& operator<<(std::ostream& out, const CombatCard& other);
	};
}