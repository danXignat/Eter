import MageCard;
import <string>;
//import<iostream>;
namespace base {
    MageCard::MageCard(MageCardType type,MageCardTypeAbility front_ability,MageCardTypeAbility back_ability, bool face_up) :
        m_mage_card_type(type), m_mage_ability_front(front_ability), m_mage_ability_back(back_ability), m_face_up(face_up) {
    }

    /*MageCardType MageCard::getType()const {
        return m_mage_card_type;
    }*/
    std::string_view MageCard::getTypeAsString() const{

        return typesToStrings(m_mage_card_type);
    }
 /*   std::string_view MageCard::getAbilityAsString() const {
        return abilityToString()
    }*/
     std::ostream& operator << (std::ostream& out, const MageCard& card) {
        out << typesToStrings(card.m_mage_card_type) << ' ' 
            << abilityToString(card.m_mage_ability_front) << ' '
            << abilityToString(card.m_mage_ability_back) << std::endl;
        return out; 
    }
};