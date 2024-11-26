#pragma once
#include <functional>

#include "Player.h"
#include "CombatCard.h"

namespace base {
	using Coord = std::pair<int16_t, int16_t>;
	using CombatCardPtr = std::unique_ptr<CombatCard>;
	using CombatCardRef = std::reference_wrapper<CombatCard>;
	using PlayerRef = std::reference_wrapper<Player>;
}