#pragma once
#include <memory>

#include "IGameMode.h"
#include "BaseGameMode.h"
#include "GameModeType.h"
#include "ServiceType.h"
#include "TrainingMode.h"

namespace base {
	using GameModePtr = std::unique_ptr<IGameMode>;

	struct GameModeFactory {
		static GameModePtr get(const std::string& id, const std::pair<std::string, std::string>& player_names);

		static GameModeType getGameModeType(const std::string& id);

		static std::vector<ServiceType> getServiceConfig(const std::string& id);
	};
}