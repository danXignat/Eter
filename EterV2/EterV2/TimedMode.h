#pragma once
#include <string>
#include <optional>
#include <memory>
#include "BaseGameMode.h"
#include "Timer.h"
#include "MageService.h"
#include "ElementalService.h"
#include "TrainingMode.h"

namespace base {
    class TimedMode : public BaseGameMode {
    public:
        // constructor pt training
        TimedMode(const std::vector<ServiceType>& services,
            const std::pair<std::string, std::string>& player_names,
            int time_limit_seconds,
            std::unique_ptr<BaseGameMode> base_mode);

        // constructor pt mage/elemental
        TimedMode(const std::vector<ServiceType>& services,
            const std::pair<std::string, std::string>& player_names,
            int time_limit_seconds = 60);

        void run() override;
        void render();

    private:
        void renderTimers();
        void switchTimers();
        void displayTimeSelection();
        void renderGameState();
        bool processGameInput(const InputHandler& input);

        std::unique_ptr<Timer> m_red_timer;
        std::unique_ptr<Timer> m_blue_timer;
        std::unique_ptr<MageService> m_mage_service;
        std::unique_ptr<ElementalService> m_elemental_service;
        std::unique_ptr<BaseGameMode> m_base_mode; 
        int m_time_limit;
    };
}