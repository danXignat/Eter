#include "TimedMode.h"
#include "InputHandler.h"
#include "logger.h"
#include <iostream>
#include <conio.h>
#include <thread>

using namespace logger;

namespace base {
    TimedMode::TimedMode(const std::vector<ServiceType>& services,
        const std::pair<std::string, std::string>& player_names,
        int time_limit_seconds,
        std::unique_ptr<BaseGameMode> base_mode)
        : BaseGameMode(base_mode ? base_mode->getBoard().getSize() == 3 ? GameSizeType::SMALL : GameSizeType::BIG : GameSizeType::SMALL,
            player_names, services),
        m_base_mode(std::move(base_mode)),
        m_time_limit(time_limit_seconds) {

        m_red_timer = std::make_unique<Timer>(time_limit_seconds);
        m_blue_timer = std::make_unique<Timer>(time_limit_seconds);
        m_board = &const_cast<Board&>(m_base_mode->getBoard());
    }

    void TimedMode::displayTimeSelection() {
        system("cls");
        std::cout << "\nSelect time limit per round (seconds):\n";
        std::cout << "1. 60 seconds\n";
        std::cout << "2. 90 seconds\n";
        std::cout << "3. 120 seconds\n";
        std::cout << "Choose an option (1-3): ";
    }

    void TimedMode::renderGameState() {
        system("cls");

        m_board->render();
        m_board->sideViewRender();

        m_player_red.renderCards();
        m_player_blue.renderCards();

        if (auto* mageMode = dynamic_cast<MageMode*>(m_base_mode.get())) {
            mageMode->render();
        }
        else if (auto* elementalMode = dynamic_cast<ElementalMode*>(m_base_mode.get())) {
            elementalMode->render();
        }
        else if (auto* trainingMode = dynamic_cast<TrainingMode*>(m_base_mode.get())) {
            trainingMode->render();
        }

        renderTimers();

        if (m_curr_player.get().getColor() == color::ColorType::RED) {
            std::cout << color::to_string(color::ColorType::RED);
            utils::printAtCoordinate("RED turn", { 1, 12 });
            std::cout << color::to_string(color::ColorType::DEFAULT);
        }
        else {
            std::cout << color::to_string(color::ColorType::BLUE);
            utils::printAtCoordinate("BLUE turn", { 1, 12 });
            std::cout << color::to_string(color::ColorType::DEFAULT);
        }

        if (m_explosion_service) {
            m_explosion_service->renderExplosion();
        }
    }

    void TimedMode::renderTimers() {
        utils::printAtCoordinate("Time remaining:", { 50, 2 });
        std::cout << color::to_string(color::ColorType::RED);
        utils::printAtCoordinate("RED: " + std::to_string(m_red_timer->getRemainingTime()) + "s", { 50, 3 });
        std::cout << color::to_string(color::ColorType::DEFAULT);
        std::cout << color::to_string(color::ColorType::BLUE);
        utils::printAtCoordinate("BLUE: " + std::to_string(m_blue_timer->getRemainingTime()) + "s", { 50, 4 });
        std::cout << color::to_string(color::ColorType::DEFAULT);
    }

    void TimedMode::switchTimers() {
        if (m_curr_player.get().getColor() == color::ColorType::RED) {
            m_red_timer->stop();
            m_blue_timer->reset();
            m_blue_timer->start();
        }
        else {
            m_blue_timer->stop();
            m_red_timer->reset();
            m_red_timer->start();
        }
    }

    bool TimedMode::processGameInput(const InputHandler& input) {
        if (m_base_mode) {
            switch (input.event_type) {
            case EventType::UseMage:
                if (auto* mageMode = dynamic_cast<MageMode*>(m_base_mode.get())) {
                    return mageMode->useMage();
                }
                break;

            case EventType::UsePower:
                if (auto* elementalMode = dynamic_cast<ElementalMode*>(m_base_mode.get())) {
                    return elementalMode->usePower();
                }
                break;

            case EventType::PlaceCombatCard:
                return placeCombatCard(input);
            case EventType::PlaceIllusion:
                return placeIllusion(input);
            }
        }

        return false;
    }

    void TimedMode::run() {
        displayTimeSelection();
        int choice;
        std::cin >> choice;

        switch (choice) {
        case 1:
            m_time_limit = 60;
            break;
        case 2:
            m_time_limit = 90;
            break;
        case 3:
            m_time_limit = 120;
            break;
        default:
            std::cout << "Invalid choice, defaulting to 60 seconds\n";
            m_time_limit = 60;
            break;
        }

        m_red_timer = std::make_unique<Timer>(m_time_limit);
        m_blue_timer = std::make_unique<Timer>(m_time_limit);
        m_red_timer->start();

        while (!m_base_mode->getBoard().getWinCoord().has_value()) {
            renderGameState();

            if (m_red_timer->hasTimeExpired() || m_blue_timer->hasTimeExpired()) {
                system("cls");
                std::cout << (m_red_timer->hasTimeExpired() ? "RED player's time has expired! BLUE wins!\n" : "BLUE player's time has expired! RED wins!\n");
                break;
            }

            if (_kbhit()) {
                InputHandler input;
                try {
                    input.read();
                    bool action_succeeded = processGameInput(input);
                    if (action_succeeded) {
                        switchTimers();
                        switchPlayer();
                    }
                }
                catch (const std::runtime_error& err) {
                    Logger::log(Level::ERROR, err.what());
                    continue;
                }
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        m_red_timer->stop();
        m_blue_timer->stop();

        if (m_base_mode->getBoard().getWinCoord().has_value()) {
            std::cout << (m_curr_player.get().getColor() == color::ColorType::BLUE ? "Player RED has won!\n" : "Player BLUE has won!\n");
        }

        std::cin.get();
        std::cin.get();
    }
}

