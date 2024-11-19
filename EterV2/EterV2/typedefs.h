#pragma once

#include <tuple>
#include <memory>

#include "CombatCard.h"
namespace base {
	using Coord = std::pair<int16_t, int16_t>;
	using CardPtr = std::unique_ptr<base::CombatCard>;
}