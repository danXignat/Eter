module CombatCard;

import <iostream>;

import CombatCardType;

namespace base {
	CombatCard::CombatCard(CombatCardType type): m_type{type} {

	}
	CombatCardType CombatCard::getType() {
		return m_type;
	}

	bool CombatCard:: operator<(const CombatCard & other) const {
		return m_type < other.m_type;
	}

	bool CombatCard::operator==(const CombatCard& other)const {
		return m_type == other.m_type;
	}
	
	std::ostream& operator<<(std::ostream& out, const CombatCard& other) {
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
		default:
			break;
		}

		return out;
	}
}
