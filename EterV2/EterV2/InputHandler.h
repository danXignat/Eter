#pragma once
#include <tuple>

#include "ServiceType.h"
#include "CombatCardType.h"
#include "GameModeType.h"
#include "colors.h"

namespace base {
	struct InputHandler {
		InputHandler();
		void read();

		std::pair<uint16_t, uint16_t> coord;
		color::ColorType color;
		uint16_t ID;
		CombatCardType card_type;
		ServiceType service_type;
		EventType event_type;
	};
	
}