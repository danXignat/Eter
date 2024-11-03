import Player;
import CardDemo;
import <string>;
import <set>;
import <iostream>;

Player::Player() = default; 

std::string_view Player::getName() const {
	return m_name;
}

const std::multiset<Card>& Player::getCards()const {
	return m_cards;
}

void Player::addCard(const Card& card) {
	m_cards.insert(card);
}

void Player::printCard() const {
	for (const auto& card : m_cards) {
		std::cout << card.getValue() << " ";//metoda implementata in card
	}
	std::cout << std::endl;
}

bool Player::hasCard() const {
	return !m_cards.empty();
}

bool Player::eraseCard(const Card& card) {
	auto it = m_cards.find(card);
	if (it != m_cards.end()) {
		m_cards.erase(it);
		return true;
	}
	return false;
}