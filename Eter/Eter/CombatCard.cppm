export module base.CombatCard;
import base.CombatCardType;

namespace base {
	export class CombatCard {
	private:
		CombatCardType m_combat_card_type;

	public:
		CombatCard(CombatCardType type);
		CombatCardType GetType();
	};
}