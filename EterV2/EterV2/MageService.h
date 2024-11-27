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
        static constexpr const int mage_number = 4;

        MageService(Board& board);
        void selectMages();
        void apply(Player& player);

    private:
        MageType _getTypeChoice(uint16_t choice);
        std::unique_ptr<MageCard> _factory(MageTypeAbility ability_type);
        std::pair<MageTypeAbility, MageTypeAbility> _getAbilityForType(MageType type);
        void _setPlayerMage(bool player);
    private:
        Board& m_board;

        MageType m_type_p1;
        MageType m_type_p2;

        std::unique_ptr<MageCard> m_card_p1;
        std::unique_ptr<MageCard> m_card_p2;
    };
}