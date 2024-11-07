module Player;

import CombatCard; 
import CombatCardType;

import <iostream>;

namespace player {

    Player::Player(std::string_view name, GameMode mode)
        : m_name{ name } {
        _initializeCards(mode);
    }

    Player::~Player() {}

    std::string_view Player::getName() const {
        return m_name;
    }

    std::unordered_multimap<base::CombatCardType, Player::CardPtr> Player::getCard() const {
        return m_cards;
    }

    void Player::addCard(std::unique_ptr<base::CombatCard> card) {
        base::CombatCardType type = card->getType();
        m_cards.emplace(type, std::move(card));
    }

    bool Player::hasCard() const {
        return !m_cards.empty();
    }

    std::unique_ptr<base::CombatCard> Player::eraseCard(base::CombatCardType type) {
        auto it = m_cards.find(type);
        if (it != m_cards.end()) {
            std::unique_ptr<base::CombatCard> removedCard = std::move(it->second);
            m_cards.erase(it);
            return removedCard;
        }
        return nullptr;
    }

    void Player::_initializeCards(GameMode mode) {
        if (mode == GameMode::TrainingMode) {
            addCard(std::make_unique<base::CombatCard>(base::CombatCardType::ONE));
            addCard(std::make_unique<base::CombatCard>(base::CombatCardType::ONE));
            addCard(std::make_unique<base::CombatCard>(base::CombatCardType::TWO));
            addCard(std::make_unique<base::CombatCard>(base::CombatCardType::TWO));
            addCard(std::make_unique<base::CombatCard>(base::CombatCardType::THREE));
            addCard(std::make_unique<base::CombatCard>(base::CombatCardType::THREE));
            addCard(std::make_unique<base::CombatCard>(base::CombatCardType::FOUR));
        }
        else if (mode == GameMode::MageDuel) {
            _initializeCards(GameMode::TrainingMode);
            addCard(std::make_unique<base::CombatCard>(base::CombatCardType::TWO));
            addCard(std::make_unique<base::CombatCard>(base::CombatCardType::THREE));
            addCard(std::make_unique<base::CombatCard>(base::CombatCardType::ETER));
        }
    }

    void Player::showCards() const {
        if (m_cards.empty()) {
            std::cout << m_name << " has no cards in inventory.\n";
            return;
        }

        std::cout << "Cards inventory for " << m_name << ":\n";
        for (const auto& [type, cardPtr] : m_cards) {
            if (cardPtr) {
                std::cout << *cardPtr << " ";
            }
        }
        std::cout << std::endl;
    }
}
