#include "GameModeFactory.h"
#include "MageMode.h"
#include "ElementalMode.h"

namespace base {
    GameModePtr GameModeFactory::get(const std::string& id, const std::pair<std::string, std::string>& player_names, int time_limit_seconds) {
        auto services = GameModeFactory::getServiceConfig(id);

        //  pt timedmode (5) 
        if (id.length() > 1 && static_cast<GameModeType>(id[0] - '0') == GameModeType::TimedMode) {
            auto secondary_mode = static_cast<GameModeType>(id[1] - '0');
            std::unique_ptr<BaseGameMode> base_mode;

            switch (secondary_mode) {
            case GameModeType::TrainingMode:
                base_mode = std::make_unique<TrainingMode>(services, player_names);
                break;

            case GameModeType::MageMode:
                base_mode = std::make_unique<MageMode>(services, player_names);
                break;

            case GameModeType::ElementalPowerMode:
                base_mode = std::make_unique<ElementalMode>(services, player_names);
                break;

            default:
                //training mode default
                base_mode = std::make_unique<TrainingMode>(services, player_names);
                break;
            }

            return std::make_unique<TimedMode>(services, player_names, time_limit_seconds, std::move(base_mode));
        }

        
        switch (static_cast<GameModeType>(id[0] - '0')) {
        case GameModeType::TrainingMode:
            return std::make_unique<TrainingMode>(services, player_names);

        case GameModeType::MageMode:
            return std::make_unique<MageMode>(services, player_names);

        case GameModeType::ElementalPowerMode:
            return std::make_unique<ElementalMode>(services, player_names);

        default:
            return nullptr;
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