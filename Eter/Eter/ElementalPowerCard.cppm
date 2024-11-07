export module ElementalPowerCard;
import ElementalPowerCardType;
import<cstdint>;
import<string>;
import<iostream>;

namespace base {
	export class ElementalPowerCard {

	private:
		ElementalPowerCardType m_type;

	public:
		ElementalPowerCard(ElementalPowerCardType type);
		ElementalPowerCardType getType()const;
		std::string_view getTypeAsString()const;

		friend std::ostream& operator << (std::ostream& out, const ElementalPowerCard& element);
	};
}