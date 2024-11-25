#include "MageCard.h"

namespace base {
    std::string_view MageCard::getTypeName()const {
        return typeToStrings(m_type) ;
    }

    std::string_view MageCard::getAbilityName() const {
        return abilityToString(m_ability);
    }
   
}