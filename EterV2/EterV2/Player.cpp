#include "Player.h"

#include "utils.h"

namespace base {

    //--------------------------------------Constructors Destructor----------------------------------

    Player::Player(std::string_view name, color::ColorType color) :
        m_name{ name },
        m_color{ color },
        m_illusion_used{ false } {
        this->_initializeCards();
    }

    //-------------------------------------------Initialisation------------------------------------

    void Player::_addCard(CombatCardType type, uint16_t count = 1) {
        for (uint16_t i = 0; i < count; i++) {
            m_cards.emplace(type, CombatCard(type, m_color));
        }
    }

    void Player::_initializeCards() {
        using enum CombatCardType;

        this->_addCard(ONE, 2);
        this->_addCard(TWO, 2);
        this->_addCard(THREE, 2);
        this->_addCard(FOUR);

        /*
        if (mode != GameModeTypes::Training) {
            this->_addCard(TWO);
            this->_addCard(THREE);
            this->_addCard(ETER);
        }
        */
    }


    //---------------------------------------Setters Getters---------------------------------------

    std::string Player::getName() const {
        return m_name;
    }

    color::ColorType Player::getColor() const {
        return m_color;
    }

    void Player::setName(std::string_view name) {
        m_name = name;
    }

    bool Player::hasCards() const {
        return !m_cards.empty();
    }

    std::optional<CombatCard> Player::getCard(CombatCardType type) { // getting card ptr or nothing if there is no card
        auto it = m_cards.find(type);

        if (it == m_cards.end()) {
            return std::nullopt;
        }

        CombatCard card = std::move(it->second);
        m_cards.erase(it);

        return card;
    }

    void Player::addCard(CombatCard&& card) {
        m_cards.emplace(card.getType(), std::move(card));
    }

    //---------------------------------------------------------------------

    void Player::renderCards() const {
        if (m_color == color::ColorType::RED) {
            utils::printAtCoordinate("Player RED cards: ", 50, 2);

            int i = 0;
            for (const auto&[type, card] : m_cards) {
                utils::printAtCoordinate(card, 50 + i, 3);
                i += 2;
            }
        }
        else {
            utils::printAtCoordinate("Player BLUE cards: ", 50, 4);

            int i = 0;
            for (const auto& [type, card] : m_cards) {
                utils::printAtCoordinate(card, 50 + i, 5);
                i += 2;
            }

        }
    }
}