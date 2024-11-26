#include "PowerCard.h"

namespace base {
	std::string_view PowerCard::getAbilityName()const {
		return typeToStrings(m_ability);
	}
}