module base.CombatCard;

namespace base {
	CombatCard::CombatCard(CombatCardType type): m_combat_card_type{type}
	{
	}
	CombatCardType CombatCard::getType() {
		return m_combat_card_type;
	}
		bool CombatCard:: operator<(const CombatCard & other) const {
			return m_combat_card_type < other.m_combat_card_type;
		}
		bool CombatCard::operator==(const CombatCard& other)const {
			return m_combat_card_type == other.m_combat_card_type;
		}
	
}
