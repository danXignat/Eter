export module MageCard;
import MageCardType;
import MageCardTypeAbility; 
import <string>;
import <iostream>;

namespace base {
    export class MageCard {

    public:
        MageCard(MageCardType type, MageCardTypeAbility front_ability,MageCardTypeAbility back_ability, bool face_up=true);
       // MageCardType getType() const;
        std::string_view getTypeAsString() const;
       // std::string_view getAbilityAsString() const;*/
        friend std::ostream& operator << (std::ostream& out, const MageCard& card);

    private:
        MageCardType m_mage_card_type;
        MageCardTypeAbility m_mage_ability_front;
        MageCardTypeAbility m_mage_ability_back;
        bool m_face_up;
    };

}