#pragma once
#include <string>
#include <memory>
#include <chrono>
#include "BaseGameMode.h"
#include "Timer.h"
#include "TrainingMode.h"
#include "MageMode.h"
#include "ElementalMode.h"
#include "InputHandler.h"
#include "utils.h"
#include "colors.h"

namespace base {
    class TimedMode : public BaseGameMode {
    public:
        TimedMode(const std::vector<ServiceType>& services,
            const std::pair<std::string, std::string>& player_names,
            int time_limit_seconds,
            std::unique_ptr<BaseGameMode> base_mode);

        void run() override;

    private:
        std::unique_ptr<BaseGameMode> m_base_mode;
        std::unique_ptr<Timer> m_red_timer;
        std::unique_ptr<Timer> m_blue_timer;
        int m_time_limit;
        Board* m_board;
        void displayTimeSelection();
        void renderGameState();
        void renderTimers();
        void switchTimers();
        bool processGameInput(const InputHandler& input);
    };
}
