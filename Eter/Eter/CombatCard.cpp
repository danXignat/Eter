module CombatCard;

import <iostream>;

import CombatCardType;
import Teams;

namespace base {
	//----------------------------------------Constructor-----------------------------
	CombatCard::CombatCard(CombatCardType type, teams::Team team)
		: m_type{ type },
		m_team{ team },
		m_illusion{ false } {

	}

	//-------------------------------------------Getters Setters---------------------------------------

	CombatCardType CombatCard::getType()const {
		return m_type;
	}

	teams::Team CombatCard::getTeam() const {
		return m_team;
	}

	//-----------------------------------------Illusion-------------------------------------------

	void CombatCard::revealIllusion() {
		m_illusion = false;
	}

	void CombatCard::setIllusion() {
		m_illusion = true;
	}

	bool CombatCard::isIllusion() const {
		return m_illusion; 
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

	//--------------------------------Overloads--------------------------------

	bool CombatCard:: operator<(const CombatCard& other) const {
		return m_type < other.m_type;
	}

	bool CombatCard::operator==(const CombatCard& other)const {
		return m_type == other.m_type;
	}

	std::ostream& operator<<(std::ostream& out, const CombatCard& other) {
		out << teams::to_string(other.getTeam());

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

		out << teams::to_string(teams::Team::DEFAULT);

		return out;
	}
}
