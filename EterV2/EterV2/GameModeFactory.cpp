#include "GameModeFactory.h"

#include "MageMode.h"
#include "ElementalMode.h"

namespace base {
	GameModePtr GameModeFactory::get(const std::string& id, const std::string& name1, const std::string& name2) {
		auto services = GameModeFactory::getServiceConfig(id);

		switch (GameModeFactory::getGameModeType(id)) {
		case GameModeType::TrainingMode: {
			return std::make_unique<TrainingMode>(services, name1, name2);
		}
		case GameModeType::MageMode: {
			//return std::unique_ptr<MageDuelMode>();
			return std::make_unique<MageMode>(services, name1, name2);
		}
		case GameModeType::ElementalPowerMode: {
			return std::make_unique<ElementalMode>(services, name1, name2);
		}
		case GameModeType::TournamentMode: {
			//return std::unique_ptr<TournamentMode>();
			return nullptr;
		}
		case GameModeType::TimedMode: {
			//return std::unique_ptr<TimedMode>();
			return nullptr;
		}
		default: {
			return nullptr;
		}
		}
	}

	GameModeType GameModeFactory::getGameModeType(const std::string& id) {
		return static_cast<GameModeType>(id.front() - '0');
	}

	std::vector<ServiceType> GameModeFactory::getServiceConfig(const std::string& id) {
		std::vector<ServiceType> services;

		for (auto it = id.begin() + 1; it != id.end(); it++) {
			services.push_back(static_cast<ServiceType>(*it - '0'));
		}

		return services;
	}
}
