#pragma once
#include <memory>

#include "IGameMode.h"
#include "GameModeType.h"
#include "ServiceType.h"
#include "TrainingMode.h"

namespace base {
	using GameModePtr = std::unique_ptr<IGameMode>;

	struct GameModeFactory {
		static GameModePtr get(const std::string& id, const std::string& name1, const std::string& name2);

		static GameModeType getGameModeType(const std::string& id);

		static std::vector<ServiceType> getServiceConfig(const std::string& id);
	};
}