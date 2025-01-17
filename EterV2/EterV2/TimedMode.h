// TimedMode.h
#pragma once
#include <string>
#include <optional>
#include <memory>
#include "BaseGameMode.h"
#include "Timer.h"

namespace base {
    class TimedMode : public BaseGameMode {
    public:
        TimedMode(const std::vector<ServiceType>& services,
            const std::pair<std::string, std::string>& player_names,
            int time_limit_seconds,
            std::unique_ptr<BaseGameMode> base_mode);

        void run() override;
        void render();
        const Player& getPlayerRed() const override { return m_base_mode->getPlayerRed(); }
        const Player& getPlayerBlue() const override { return m_base_mode->getPlayerBlue(); }
        const Player& getCurrPlayer() const override { return m_base_mode->getCurrPlayer(); }
        const Board& getBoard() const override { return m_base_mode->getBoard(); }

    private:
        std::unique_ptr<Timer> m_red_timer;
        std::unique_ptr<Timer> m_blue_timer;
        void renderTimers();
        void switchTimers();
        int m_time_limit;
        void displayTimeSelection();
        void renderGameState();
        bool processGameInput(const InputHandler& input);
        std::unique_ptr<BaseGameMode> m_base_mode;
    };
}