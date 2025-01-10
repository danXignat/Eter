#pragma once
#include <string>
#include <optional>
#include <memory>
#include "BaseGameMode.h"
#include "Timer.h"

namespace base {
    class TimedMode : public BaseGameMode {
    public:
        TimedMode(const std::vector<ServiceType>&, const std::pair<std::string, std::string>&, int time_limit_seconds = 60);
        void run() override;
        void render();

    private:
        std::unique_ptr<Timer> red_timer;
        std::unique_ptr<Timer> blue_timer;
        void renderTimers();
        void switchTimers();
        int time_limit;
    };
}
