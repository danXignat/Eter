#pragma once
#include <cstdint>

namespace base {
	enum class GameModeType : uint16_t {
		None, 
		TrainingMode,
		MageMode,
		ElementalPowerMode,
		TournamentMode,
		TimedMode
	};

	enum class GameSizeType : uint16_t {
		SMALL = 3,
		BIG = 4
	};
}