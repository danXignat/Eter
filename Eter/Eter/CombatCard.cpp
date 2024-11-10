module CombatCard;

import <iostream>;

import CombatCardType;
import Teams;

namespace base {
	CombatCard::CombatCard(CombatCardType type, bool illusion, teams::Team team)
		: m_type{ type },
		m_illusion{ illusion },
		m_team{ team } {

	}
	CombatCardType CombatCard::getType()const {
		return m_type;
	}

	teams::Team CombatCard::getTeam() const {
		return m_team;
	}

	void CombatCard::reveal() {
		m_illusion = false;
	}
	bool CombatCard::getIllusionCard() const {
		return m_illusion; 
	}

	void CombatCard::setIllusionCard(bool illusion) {
		m_illusion = illusion;
	}

	bool CombatCard::attemptCover(const CombatCard& opponentCard) const {
		if (m_illusion)
		{
			if (opponentCard.getType() >= m_type) {
				return false;
			}
			else {
				return true;
			}
		}
		return opponentCard.getType() < m_type;
	}

	bool CombatCard:: operator<(const CombatCard& other) const {
		return m_type < other.m_type;
	}

	bool CombatCard::operator==(const CombatCard& other)const {
		return m_type == other.m_type;
	}

	std::ostream& operator<<(std::ostream& out, const CombatCard& other) {
		if (other.m_illusion) {
			out << "X";
		}
		else {
			switch (other.m_type)
			{
				using enum CombatCardType;
			case ETER:
				out << "E";
				break;
			case ONE:
				out << "1";
				break;
			case TWO:
				out << "2";
				break;
			case THREE:
				out << "3";
				break;
			case FOUR:
				out << "4";
				break;
			default:
				break;
			}
		}
		return out;
	}
}
