#include "Player.h"

#include "utils.h"

namespace base {

    //--------------------------------------Constructors Destructor----------------------------------

    Player::Player(std::string_view name, color::ColorType color, GameSizeType game_size) :
        m_name{ name },
        m_color{ color } {
        _initializeCards(game_size);
    }

    Player::Player(Player&& other) noexcept 
        : m_color{other.m_color},
        m_name{ std::move(other.m_name) },
        m_cards{ std::move(other.m_cards) } {

        other.m_color = color::ColorType::DEFAULT;
        other.m_name.clear();
        other.m_cards.clear();
    }

    Player& Player::operator=(Player&& other) noexcept {
        if (&other != this) {
            m_color = other.m_color;
            m_name = std::move(other.m_name);
            m_cards = std::move(other.m_cards);

            other.m_color = color::ColorType::DEFAULT;
            other.m_name.clear();
            other.m_cards.clear();
        }

        return *this;
    }

    //-------------------------------------------Initialisation------------------------------------

    void Player::_addCard(CombatCardType type, uint16_t count) {
        for (uint16_t i = 0; i < count; i++) {
            m_cards.emplace(type, CombatCard(type, m_color));
        }
    }

    void Player::_initializeCards(GameSizeType game_size) {
        for (const auto& [type, count] : Player::card_configs.at(game_size)) {
            _addCard(type, count);
        }
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

    bool Player::hasCard(CombatCardType type) const { 
        return m_cards.contains(type);
    }

    bool Player::hasUsedCard(CombatCardType type) const {
        return m_used_cards.contains(type);
    }

    CombatCard&& Player::getCard(CombatCardType type) {
        if (this->hasCard(type) == false) {
            throw std::runtime_error("player does not have this card");
        }

        auto it = m_cards.find(type);

        CombatCard card = std::move(it->second);
        m_cards.erase(it);

        return std::move(card);
    }

    CombatCard&& Player::getUsedCard(CombatCardType type) {
        if (this->hasUsedCard(type) == false) {
            throw std::runtime_error("player does not have this card");
        }

        auto it = m_used_cards.find(type);

        CombatCard card = std::move(it->second);
        m_used_cards.erase(it);

        return std::move(card);
    }

    const CombatCard& Player::viewCard(CombatCardType type) {
        if (this->hasCard(type) == false) {
            throw std::runtime_error("player does not have this card");
        }

        auto it = m_cards.find(type);

        return it->second;
    }

    void Player::addCard(CombatCard&& card) {
        m_cards.emplace(card.getType(), std::move(card));
    }

    void Player::addUsedCard(CombatCard&& card) {
        m_used_cards.emplace(card.getType(), std::move(card));
    }

    void Player::setOpponent(Player* opponent) {
        m_opponent = opponent;
    }

    Player* Player::getOpponent() const {
        return m_opponent;
    }

    //---------------------------------------------------------------------

    void Player::renderCards() const {
        if (m_color == color::ColorType::RED) {
            utils::printAtCoordinate("Player RED cards: ", 1, 14);

            int i = 0;
            for (const auto&[type, card] : m_cards) {
                utils::printAtCoordinate(card, 1 + i, 15);
                i += 2;
            }
        }
        else {
            utils::printAtCoordinate("Player BLUE cards: ", 1, 16);

            int i = 0;
            for (const auto& [type, card] : m_cards) {
                utils::printAtCoordinate(card, 1 + i, 17);
                i += 2;
            }
        }
    }

    const std::unordered_map<GameSizeType, std::unordered_map<CombatCardType, uint16_t>> Player::card_configs{
    {
        GameSizeType::SMALL, {
            {CombatCardType::ONE, 2},
            {CombatCardType::TWO, 2},
            {CombatCardType::THREE, 2},
            {CombatCardType::FOUR, 1}
        }
    },
    {
        GameSizeType::BIG, {
            {CombatCardType::ONE, 2},
            {CombatCardType::TWO, 3},
            {CombatCardType::THREE, 3},
            {CombatCardType::FOUR, 1},
            {CombatCardType::ETER, 1},
        }
    }
    };
}