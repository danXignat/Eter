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
        CombatCard getCard(CombatCardType);
        const CombatCard& viewCard(CombatCardType);
        bool hasCards() const;
        bool hasCard(CombatCardType);

        void renderCards() const;
        void setName(std::string_view name);
        void addCard(CombatCard&&);

    private:
        void _initializeCards(GameSizeType);
        void _addCard(CombatCardType, uint16_t);

    private:
        color::ColorType m_color;
        std::string m_name;
        std::unordered_multimap<CombatCardType, CombatCard> m_cards;
    };

    using PlayerRef = std::reference_wrapper<Player>;
    
}