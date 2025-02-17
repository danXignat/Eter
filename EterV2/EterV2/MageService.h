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
        static constexpr const uint16_t MAGE_NUMBER = 8;

        MageService(Board& board, Player& red, Player& blue);
        bool apply(color::ColorType color);

        MageTypeAbility getMageAbility(color::ColorType color);
        void renderCard(PlayerRef player) const;

        template <typename T>
        T* getMageCard(color::ColorType color);

    private:
        std::unique_ptr<MageCard> _factory(MageTypeAbility ability_type, color::ColorType color);
 
    private:
        Board& m_board;
        Player& m_player_red;
        Player& m_player_blue;

        std::unique_ptr<MageCard> card_red;
        std::unique_ptr<MageCard> card_blue;
    };

    template <typename T>
    T* MageService::getMageCard(color::ColorType color) {
        if (color == color::ColorType::RED) {
            return dynamic_cast<T*>(card_red.get());
        }
        else {
            return dynamic_cast<T*>(card_blue.get());
        }
    }
}
