module CombatCard;

import <iostream>;

import CombatCardType;
import Teams;

namespace base {
	CombatCard::CombatCard(CombatCardType type, teams::Team team): 
		m_type{type},
		m_team{team} {

	}

	CombatCardType CombatCard::getType() {
		return m_type;
	}

	teams::Team CombatCard::getTeam() const {
		return m_team;
	}

	bool CombatCard:: operator<(const CombatCard & other) const {
		return m_type < other.m_type;
	}

	bool CombatCard::operator==(const CombatCard& other)const {
		return m_type == other.m_type;
	}
	
	std::ostream& operator<<(std::ostream& out, const CombatCard& other) {
		out << teams::to_string(other.m_team);

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

		out << "\033[0m";
		return out;
	}
}
