#include "MageCard.h"

namespace base {
    MageCard::MageCard(Board& board, Player& player_red, Player& player_blue, color::ColorType color)
        : m_type{ MageType::None },
        m_ability{ MageTypeAbility::None },
        m_color{ color },

        m_board{ board },
        m_player_red{ player_red },
        m_player_blue{ player_blue } {

    }

    MageType MageCard::getType() const {
        return m_type;
    }

    MageTypeAbility MageCard::getMasterType() const {
        return m_ability;
    }
}