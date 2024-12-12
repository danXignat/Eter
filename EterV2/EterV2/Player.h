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

        bool hasCards() const;
        bool hasCard(CombatCardType) const;
        bool hasUsedCard(CombatCardType) const;

        CombatCard&& getCard(CombatCardType);
        CombatCard&& getUsedCard(CombatCardType);
        const CombatCard& viewCard(CombatCardType);
        void addCard(CombatCard&&);
        void addUsedCard(CombatCard&&);

        void setOpponent(Player* opponent);
        Player* getOpponent() const;
        void renderCards() const;

    private:
        void _initializeCards(GameSizeType);
        void _addCard(CombatCardType, uint16_t);

    private:
        color::ColorType m_color;
        std::string m_name;

        std::unordered_multimap<CombatCardType, CombatCard> m_cards;
        std::unordered_multimap<CombatCardType, CombatCard> m_used_cards;

        Player* m_opponent = nullptr;
    };

    using PlayerRef = std::reference_wrapper<Player>;
    
}