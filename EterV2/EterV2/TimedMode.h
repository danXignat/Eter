#pragma once
#include <string>
#include <optional>
#include <memory>
#include "BaseGameMode.h"
#include "Timer.h"
#include "MageService.h"
#include "ElementalService.h"
#include "ArenaService.h"
#include "TrainingMode.h"

namespace base {
    class TimedMode : public BaseGameMode {
    public:
        // constructor pt training
        TimedMode(GameSizeType size_type, const std::vector<ServiceType>& services,
            const std::pair<std::string, std::string>& player_names,
            uint16_t time_limit);

        void run() override;
        void render();
        void setLostOnTime(color::ColorType color);
        void nextRound() override;

        std::optional <MageService>& getMageService();
        std::optional <ElementalService>& getElementalService();
        std::optional <ArenaService>& getArenaService();
        uint16_t getTime() const;

    private:
        void renderTimers();
        void switchTimers();
        void displayTimeSelection();
        void renderGameState();
        bool processGameInput(const InputHandler& input);

    private:
        uint16_t m_time_limit;

        color::ColorType lost_on_time_color = color::ColorType::DEFAULT;
        bool lost_on_time = false;

        std::optional<MageService> m_mage_service;
        std::optional<ElementalService> m_elemental_service;
        std::optional<ArenaService> m_arena_service;
    };
}