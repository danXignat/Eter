module base.CombatCard;

namespace base {
	CombatCard::CombatCard(CombatCardType type): m_combat_card_type{type}
	{
	}
	CombatCardType CombatCard::GetType() {
		return m_combat_card_type;
	}
}
