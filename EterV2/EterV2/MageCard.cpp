#include "MageCard.h"

namespace base {
    MageCard::MageCard()
        : m_type { MageType::None },
        m_ability{ MageTypeAbility::None } {
        
    }

    std::string_view MageCard::getTypeName()const {
        return typeToStrings(m_type) ;
    }

    std::string_view MageCard::getAbilityName() const {
        return abilityToString(m_ability);
    }
   
}