export module base.MageCard;
import base.MageCardType;
import <string>;

namespace base {
    export class MageCard {
    private:
        MageCardType m_mage_card_type;
    public:
        MageCard(MageCardType type);
        MageCardType getType() const;
        std::string_view getTypeAsString() const;
    };

}