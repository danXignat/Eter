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
}