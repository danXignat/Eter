#pragma once
#include <optional>

#include "ServiceType.h"
#include "CombatCardType.h"

namespace base {
	struct InputHandler {
		InputHandler() = default;
		void read();

		std::optional<uint16_t> x, y;
		std::optional<CombatCardType> card_type;
		std::optional<ServiceType> service_type;
	};
}