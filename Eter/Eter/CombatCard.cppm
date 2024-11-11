export module CombatCard;

import <iostream>;

import CombatCardType;
import Teams;

namespace base {
	export class CombatCard {

	public:
		CombatCard(CombatCardType type, teams::Team team);
		CombatCardType getType() const;

		teams::Team getTeam() const;
		void revealIllusion();
		void setIllusion();
		bool isIllusion() const;

		bool attemptCover(const CombatCard& opponentCard) const;

		bool operator<(const CombatCard& other) const;
		bool operator==(const CombatCard& other) const;

		friend std::ostream& operator<<(std::ostream& out, const CombatCard& other);

	private:
		CombatCardType m_type;
		bool m_illusion;
		teams::Team m_team;

	};
}
