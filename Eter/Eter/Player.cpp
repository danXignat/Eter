module Player;

import <iostream>;
import <string>;
import <string_view>;
import <unordered_map>;
import <memory>;

import CombatCard; 
import CombatCardType;
import GameModeTypes;

namespace base {

    //--------------------------------------Constructors Destructor----------------------------------

    Player::Player(std::string_view name, GameModeTypes mode) :
        m_name{ name } {

        this->_initializeCards(mode);
     }

    Player::~Player() {
    
    }
    
    //-------------------------------------------Initialisation------------------------------------

    void Player::_addCard(CombatCardType type, uint16_t count = 1) {
        for (uint16_t i = 0; i < count; i++) {
            m_cards.emplace(type, std::make_unique<base::CombatCard>(type));
        }
    }
    

    void Player::_initializeCards(GameModeTypes mode) {
        using enum CombatCardType;

        this->_addCard(ONE, 2);
        this->_addCard(TWO, 2);
        this->_addCard(THREE, 2);
        this->_addCard(FOUR);
        

        if (mode != GameModeTypes::Training) {
            this->_addCard(TWO);
            this->_addCard(THREE);
            this->_addCard(ETER);
        }
    }

    //---------------------------------------Setters Getters---------------------------------------

    std::string Player::getName() const {
        return m_name;
    }

    void Player::setName(std::string_view name) {
        m_name = name;
    }

    bool Player::hasCards() const {
        return !m_cards.empty();
    }

    std::optional<CardPtr> Player::getCard(CombatCardType type) { // getting card ptr or nothing if there is no card
        auto it = m_cards.find(type);
        
        if (it == m_cards.end()) {
            return std::nullopt;
        }

        CardPtr card_ptr = std::move(it->second);
        m_cards.erase(it);

        return card_ptr;
    }

    void Player::addCard(std::unique_ptr<CombatCard>&& card) {
        m_cards.emplace(card->getType(), std::move(card));
    }

}
