export module Player;
import base.CombatCard;
import base.CombatCardType;

import<string>;
import <string_view>;
import<set>;

namespace player {
	export class Player {
	public:
		Player() = default;
		~Player();
		std::string_view getName() const;
		const std::multiset<base::CombatCard>& getCards()const;

		void addCard(const base::CombatCard& card);
		//void printCard() const;
		bool hasCard() const;
		bool eraseCard(const base::CombatCard& card);

	private:
		std::string_view m_name;
		std::multiset<base::CombatCard> m_cards;

	};
}