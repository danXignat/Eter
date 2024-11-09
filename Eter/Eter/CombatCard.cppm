export module CombatCard;

import <iostream>;

import CombatCardType;
import Teams;

namespace base {
	export class CombatCard {
	public:
		CombatCard(CombatCardType type, teams::Team);
		CombatCardType getType();
		teams::Team getTeam() const;

		bool operator<(const CombatCard& other) const;
		bool operator==(const CombatCard& other) const;

		friend std::ostream& operator<<(std::ostream& out, const CombatCard& other);
	private:
		teams::Team m_team;
		CombatCardType m_type;
	};

}