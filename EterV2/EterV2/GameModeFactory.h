#pragma once
#include <memory>
#include <array>

#include "IGameMode.h"
#include "TrainingMode.h"
#include "GameModeType.h"
#include "ServiceType.h"

namespace base {
	using GameModePtr = std::unique_ptr<IGameMode>;

	struct GameModeFactory {
		static constexpr uint16_t id_length = 7;

		static GameModePtr getGameMode(const std::string&, const std::string&, const std::string&);
	
		static GameModeType getGameModeType(const std::string&);

		static std::vector<ServiceType> getSelectedServices(const std::string&);
	};
}