#pragma once
#include <memory>

#include "IGameMode.h"
#include "TrainingMode.h"

namespace base {
	using GameModePtr = std::unique_ptr<IGameMode>;

	struct GameModeFactory {
		static GameModePtr get(const std::string& id, const std::string& name1, const std::string& name2);
	};
}