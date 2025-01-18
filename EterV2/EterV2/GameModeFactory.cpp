#include "GameModeFactory.h"
#include "MageMode.h"
#include "ElementalMode.h"

namespace base {
    GameModePtr GameModeFactory::get(const std::string& id, const std::pair<std::string, std::string>& player_names, int time_limit_seconds) {
        auto services = GameModeFactory::getServiceConfig(id);

        //  pt timedmode (5) 
        if (id.length() > 1 && static_cast<GameModeType>(id[0] - '0') == GameModeType::TimedMode) { 
            auto secondary_mode = static_cast<GameModeType>(id[1] - '0'); 

            if (secondary_mode == GameModeType::TrainingMode) { 
                auto base_mode = std::make_unique<TrainingMode>(services, player_names);    
                return std::make_unique<TimedMode>(services, player_names, time_limit_seconds, std::move(base_mode)); 
            }
            else {
                std::vector<ServiceType> mode_services; 

                for (size_t i = 1; i < id.length(); i++) { 
                    auto mode_type = static_cast<GameModeType>(id[i] - '0'); 
                    switch (mode_type) { 
                    case GameModeType::MageMode: 
                        mode_services.push_back(ServiceType::MAGE); 
                        break; 
                    case GameModeType::ElementalPowerMode: 
                        mode_services.push_back(ServiceType::ELEMENTAL); 
                        break;
                    default:
                        break;
                    }
                }

                return std::make_unique<TimedMode>(mode_services, player_names, time_limit_seconds);
            }
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