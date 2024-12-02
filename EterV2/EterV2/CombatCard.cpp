#include "CombatCard.h"

namespace base {
	//----------------------------------------Constructor-----------------------------
	CombatCard::CombatCard()
		: m_type{ CombatCardType::NONE },
		m_color{ color::ColorType::DEFAULT },
		m_illusion{ false } {

	}

	CombatCard::CombatCard(CombatCardType type, color::ColorType color)
		: m_type{ type },
		m_color{ color },
		m_illusion{ false } {

	}

	CombatCard::CombatCard(CombatCard&& other) noexcept {
		this->m_type = other.m_type;
		this->m_color = other.m_color;
		this->m_illusion = other.m_illusion;
	}

	CombatCard& CombatCard::operator=(CombatCard&& other) noexcept {
		if (this != &other) {
			this->m_type = other.m_type;
			this->m_color = other.m_color;
			this->m_illusion = other.m_illusion;
		}

		return *this;
	}

	//-------------------------------------------Getters Setters---------------------------------------

	CombatCardType CombatCard::getType()const {
		return m_type;
	}

	color::ColorType CombatCard::getColor() const {
		return m_color;
	}

	//-----------------------------------------Illusion-------------------------------------------

	void CombatCard::flip() {
		m_illusion = !m_illusion;
	}
	
	bool CombatCard::isIllusion() const {
		return m_illusion;
	}

	//--------------------------------Overloads--------------------------------

	bool CombatCard:: operator<(const CombatCard& other) const {
		return m_type < other.m_type;
	}

	bool CombatCard:: operator>=(const CombatCard& other) const {
		return m_type >= other.m_type;
	}

	bool CombatCard::operator==(const CombatCard& other)const {
		return m_type == other.m_type;
	}

	std::ostream& operator<<(std::ostream& out, const CombatCard& other) {
		out << color::to_string(other.getColor());

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

		out << color::to_string(color::ColorType::DEFAULT);

		return out;
	}
}
