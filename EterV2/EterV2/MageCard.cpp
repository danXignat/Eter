#include "MageCard.h"

namespace base {
    MageCard::MageCard()
        : m_type { MageType::None },
        m_ability{ MageTypeAbility::None } {
        
    }

    MageType MageCard::getType() const {
        return m_type;
    }

    MageTypeAbility MageCard::getMasterType() const {
        return m_ability;
    }
}