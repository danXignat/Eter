import ElementalPowerCardType;
import ElementalPowerCard;

namespace base {
	ElementalPowerCard::ElementalPowerCard(ElementalPowerCardType type) :m_type(type) {
	}

	ElementalPowerCardType ElementalPowerCard::getType() const {
		return m_type;
	}

	std::string_view ElementalPowerCard::getTypeAsString() const {
		return mapPowers.at(m_type);
	}
	std::ostream& operator<<(std::ostream& out, const ElementalPowerCard& element) {
		out << element.getTypeAsString();
		out << std::endl;
		return out;
	}
}