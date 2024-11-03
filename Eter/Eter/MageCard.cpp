import base.MageCard;
import <string>;
namespace base {
    MageCard::MageCard(MageCardType type) :m_mage_card_type(type) {}

    MageCardType MageCard::getType()const {
        return m_mage_card_type;
    }
    std::string_view MageCard::getTypeAsString() const
    {
        return typesToStrings(m_mage_card_type);
    }
};