#include "CombatCard.h"

#include "logger.h"

namespace base {
	int16_t CombatCard::card_count = 0;

	//----------------------------------------Constructor-----------------------------
	CombatCard::CombatCard()
		: m_type{ CombatCardType::NONE },
		m_color{ color::ColorType::DEFAULT },
		m_illusion{ false },
		m_ID{-1} {
		logger::Logger::log(logger::Level::INFO, "card created ID {}", m_ID);
	}

	CombatCard::CombatCard(CombatCardType type, color::ColorType color)
		: m_type{ type },
		m_color{ color },
		m_illusion{ false } {

		if (type == CombatCardType::ONE ||
			type == CombatCardType::TWO ||
			type == CombatCardType::THREE ||
			type == CombatCardType::FOUR) {
			m_ID = card_count;
			card_count++;
		}
		else {
			m_ID = -1;
		}

		logger::Logger::log(logger::Level::INFO, "card created ID {}", m_ID);
	}

	CombatCard::CombatCard(CombatCard&& other) noexcept {
		this->m_type = other.m_type;
		this->m_color = other.m_color;
		this->m_illusion = other.m_illusion;
		this->m_ID = other.m_ID;
	}

	CombatCard& CombatCard::operator=(CombatCard&& other) noexcept {
		if (this != &other) {
			this->m_type = other.m_type;
			this->m_color = other.m_color;
			this->m_illusion = other.m_illusion;
			this->m_ID = other.m_ID;
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

	uint16_t CombatCard::getID() const {
		return m_ID;
	}

	//-----------------------------------------Illusion-------------------------------------------

	void CombatCard::flip() {
		m_illusion = !m_illusion;
	}
	
	bool CombatCard::isIllusion() const {
		return m_illusion;
	}

	bool CombatCard::isCombatCard() const {
		return (m_ID != -1) ? true : false;
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
			case HOLE:
				out << "H";
				break;
			default:
				break;
			}
		}

		out << color::to_string(color::ColorType::DEFAULT);

		return out;
	}
}
