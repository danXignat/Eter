#include "Player.h"

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


        /*if (mode != GameModeTypes::Training) {
            this->_addCard(TWO);
            this->_addCard(THREE);
            this->_addCard(ETER);
        }*/
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

    std::optional<CombatCard> Player::getCard(CombatCardType type, bool illusion) { // getting card ptr or nothing if there is no card
        if (illusion) {
            if (m_illusion_used) {
                return std::nullopt;
            }
            else {
                m_illusion_used = true;
            }
        }

        auto it = m_cards.find(type);

        if (it == m_cards.end()) {
            return std::nullopt;
        }

        CombatCard card = std::move(it->second);
        m_cards.erase(it);

        if (m_illusion_used) {
            card.setIllusion();
        }

        return card;
    }

    void Player::addCard(CombatCard&& card) {
        m_cards.emplace(card.getType(), std::move(card));
    }

    /*void Player::setMageCard(std::unique_ptr<MageCard>&& mageCard) {
        m_mage_card = std::move(mageCard);
    }

    MageCard* Player::getMageCard() {
        return m_mage_card.get();
    }

    bool Player::useMageCard() {
        if (!m_mage_card_used && m_mage_card) {
            m_mage_card_used = true;
            return true;
        }
        return false;
    }

    bool Player::hasUnusedMageCard() const
    {
        return m_mage_card && !m_mage_card_used;
    }*/

}