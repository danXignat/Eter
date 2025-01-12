#pragma once

#include <string>
#include <vector>
#include <memory>
#include "BaseGameMode.h"
#include "Timer.h"
#include "ServiceType.h"

namespace base {
    class TimedMode : public BaseGameMode {
    public:
        TimedMode(const std::vector<ServiceType>& services,
            const std::pair<std::string, std::string>& player_names,
            int time_limit_seconds = 60);

        void run() override;
        void render();

    private:
        std::unique_ptr<Timer> red_timer;
        std::unique_ptr<Timer> blue_timer;
        int time_limit;

        void renderTimers();
        void switchTimers();
    };
}
