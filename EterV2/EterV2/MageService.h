#pragma once
#include <vector> 
#include <memory>
#include <array>
#include <numeric>
#include <algorithm>

#include "MageCard.h"
#include "MageTypeAbility.h"

#include "Masters.h"

namespace base {
    class MageService {
    public:
        static constexpr const uint16_t mage_number = 8;

        MageService(Board& board);
        bool apply(Player& player);
        void renderCard(PlayerRef player) const;

    private:
        std::unique_ptr<MageCard> _factory(MageTypeAbility ability_type);
    private:
        Board& m_board;

        std::unique_ptr<MageCard> m_card_p1;
        std::unique_ptr<MageCard> m_card_p2;
    };
}