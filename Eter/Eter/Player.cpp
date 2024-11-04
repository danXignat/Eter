import Player;
import CombatCard;
import CombatCardType;
import <string>;
import <set>;
import <iostream>;

namespace player {
	Player::~Player() {}

	std::string_view Player::getName() const {
		return m_name;
	}

	const std::multiset<base::CombatCard>& Player::getCards()const {
		return m_cards;
	}

	void Player::addCard(const base::CombatCard& card) {
		m_cards.insert(card);
	}

	bool Player::hasCard() const {
		return !m_cards.empty();
	}

	bool Player::eraseCard(const base::CombatCard& card) {
		auto it = m_cards.find(card);
		if (it != m_cards.end()) {
			m_cards.erase(it);
			return true;
		}
		return false;
	}
}