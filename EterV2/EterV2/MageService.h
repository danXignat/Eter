#pragma once
#include <vector> 
#include <memory>
#include <array>
#include <numeric>
#include <algorithm>

#include "MageCard.h"
#include "MageTypeAbility.h"

#include "MasterOfFire.h"
#include "MasterOfAir.h"
#include "MasterOfWater.h"
#include "MasterOfEarth.h"

namespace base {
    class MageService {
    public:
        static constexpr const int mage_number = 4;

        MageService(Board& board);
        void selectMages();
        MageType _getTypeChoice(uint16_t choice);

    private:
        std::unique_ptr<MageCard> _factory(MageTypeAbility ability_type);
        std::pair<MageTypeAbility, MageTypeAbility> _getAbilityForType(MageType type);
        void _setPlayerMage(bool player);
    private:
        Board& m_board;

        MageType type_p1;
        MageType type_p2;

        std::unique_ptr<MageCard> card_p1;
        std::unique_ptr<MageCard> card_p2;
    };
}