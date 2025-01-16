#include "GameModeFactory.h"
#include "MageMode.h"
#include "ElementalMode.h"

namespace base {
    GameModePtr GameModeFactory::get(const std::string& id, const std::pair<std::string, std::string>& player_names, int seconds_limit) {
        auto services = GameModeFactory::getServiceConfig(id);
        auto primary_mode = static_cast<GameModeType>(id[0] - '0');

        // pentru TimedMode (5)
        if (primary_mode == GameModeType::TimedMode && id.length() > 1) {
            std::unique_ptr<BaseGameMode> base_mode;
            auto secondary_mode = static_cast<GameModeType>(id[1] - '0');

            switch (secondary_mode) {
            case GameModeType::MageMode: // 2
                base_mode = std::make_unique<MageMode>(services, player_names);
                break;
            case GameModeType::ElementalPowerMode: // 3
                base_mode = std::make_unique<ElementalMode>(services, player_names);
                break;
            case GameModeType::TrainingMode: // 1
            default:
                base_mode = std::make_unique<TrainingMode>(services, player_names);
                break;
            }
            return std::make_unique<TimedMode>(services, player_names, seconds_limit, std::move(base_mode));
        }

        // Pentru moduri non-timed
        switch (primary_mode) {
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