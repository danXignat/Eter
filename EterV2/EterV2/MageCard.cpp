#include "MageCard.h"
namespace base {
    MageCard::MageCard(MageType type, MageTypeAbility front_ability, MageTypeAbility back_ability, bool face_up) :
        m_mage_type(type),
        m_mage_front_ability(front_ability),
        m_mage_back_ability(back_ability),
        m_face_up(face_up) {
    }

    std::string_view MageCard::getTypeName()const
    {
        return typeToStrings(m_mage_type) ;
    }

    std::string_view MageCard::getFrontAbilityName()const
    {
        return abilityToString(m_mage_front_ability);
    }

    std::string_view MageCard::getBackAbilityName()const
    {
        return abilityToString(m_mage_back_ability);
    }

   
}