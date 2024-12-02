#pragma once
#include <tuple>

#include "ServiceType.h"
#include "CombatCardType.h"
#include "GameModeType.h"

namespace base {
	struct InputHandler {
		InputHandler();
		void read();

		std::pair<uint16_t, uint16_t> coord;
		CombatCardType card_type;
		ServiceType service_type;
		GameEventType event_type;
	};
	
}