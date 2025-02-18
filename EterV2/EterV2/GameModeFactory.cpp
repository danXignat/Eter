#include "GameModeFactory.h"

namespace base {
    GameModePtr GameModeFactory::get(const std::string& id, const std::pair<std::string, std::string>& player_names, int time_limit_seconds) {
        auto services = GameModeFactory::getServiceConfig(id);

        switch (getGameModeType(id)) {

        case GameModeType::TrainingMode:
            return std::make_unique<TrainingMode>   (services, player_names);

        case GameModeType::MageMode:
            return std::make_unique<MageMode>       (services, player_names);

        case GameModeType::ElementalPowerMode:
            return std::make_unique<ElementalMode>  (services, player_names);

        case GameModeType::TournamentMode:
            return std::make_unique<TournamentMode>  (services, player_names);

        /*case GameModeType::TimedMode:
            return std::make_unique<TimedMode>      (services, player_names);*/

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