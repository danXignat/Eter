#pragma once
#include <unordered_map>
#include <memory>
#include <string_view>
#include <optional>

#include "CombatCard.h"
#include "colors.h"
#include "GameModeType.h"

namespace base {
    class Player {
    public:
        static const std::unordered_map<GameSizeType, std::unordered_map<CombatCardType, uint16_t>> card_configs;

        Player(std::string_view, color::ColorType, GameSizeType);

        Player(const Player&) = delete;
        Player& operator=(const Player&) = delete;

        Player(Player&&) noexcept;
        Player& operator=(Player&&) noexcept;

        std::string getName() const;
        color::ColorType getColor() const;
        void setName(std::string_view name);
        const std::unordered_multimap<CombatCardType, CombatCard>& getCards() const;
        const std::unordered_multimap<CombatCardType, CombatCard>& getUsedCards() const;

        bool hasCards() const;
        bool hasCard(CombatCardType) const;
        bool hasCard(uint16_t) const;
        bool hasUsedCard(CombatCardType) const;
        bool hasUsedCards() const;

        CombatCard&& getCard(CombatCardType);
        CombatCard&& getCardByID(uint16_t);
        CombatCard&& getUsedCard(CombatCardType);
        CombatCard&& getUsedCardByID(uint16_t);
        const CombatCard& viewCard(CombatCardType);
        void addCard(CombatCard&&);
        void addUsedCard(CombatCard&&);

        void renderCards() const;
        void renderUsedCards() const;
        void reset();

    private:
        void _initializeCards(GameSizeType);
        void _addCard(CombatCardType, uint16_t);

    private:
        static uint16_t card_id_counter;

        color::ColorType m_color;
        std::string m_name;

        std::unordered_multimap<CombatCardType, CombatCard> m_cards;
        std::unordered_multimap<CombatCardType, CombatCard> m_used_cards;

    };

    using PlayerRef = std::reference_wrapper<Player>;
    
}