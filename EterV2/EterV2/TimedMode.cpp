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
        : BaseGameMode(base_mode->getBoard().getSize() == 3 ? GameSizeType::SMALL : GameSizeType::BIG,
            player_names, services)
        , m_time_limit(time_limit_seconds)
        , m_base_mode(std::move(base_mode))
    {
    }
    
    TimedMode::TimedMode(const std::vector<ServiceType>& services,
        const std::pair<std::string, std::string>& player_names,
        int time_limit_seconds)
        : BaseGameMode(GameSizeType::BIG, player_names, services)
        , m_time_limit(time_limit_seconds)
    {
    
        for (const auto& service : services) {
            if (service == ServiceType::MAGE) {
                m_mage_service = std::make_unique<MageService>(m_board);
            }
            else if (service == ServiceType::ELEMENTAL) {
                m_elemental_service = std::make_unique<ElementalService>(m_board);
            }
        }
    }

    void TimedMode::renderGameState() {
        system("cls");

        if (auto* trainingMode = dynamic_cast<TrainingMode*>(m_base_mode.get())) {
            trainingMode->render();
        }
        else {
            m_board.render();
            m_board.sideViewRender();
            m_player_red.renderCards();
            m_player_blue.renderCards();

            if (m_mage_service) {
                m_mage_service->renderCard(m_curr_player);
            }
            if (m_elemental_service) {
                m_elemental_service->renderCards();
            }
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

    bool TimedMode::processGameInput(const InputHandler& input) {
        bool action_succeeded = false;

        if (m_base_mode) {
           
            switch (input.event_type) {
            case EventType::PlaceCombatCard:
            case EventType::PlaceIllusion:
                action_succeeded = m_base_mode->placeCombatCard(input);
                break;
            default:
                break;
            }
        }
        else {
            
            switch (input.event_type) {
            case EventType::UseMage:
                if (m_mage_service) {
                    action_succeeded = m_mage_service->apply(m_curr_player.get());
                }
                break;

            case EventType::UsePower:
                if (m_elemental_service) {
                    char choice;
                    std::cin >> choice;
                    m_elemental_service->apply(choice, m_curr_player.get());
                    action_succeeded = true;
                }
                break;

            case EventType::PlaceCombatCard:
                action_succeeded = placeCombatCard(input);
                break;

            case EventType::PlaceIllusion:
                action_succeeded = placeIllusion(input);
                break;

            default:
                break;
            }
        }

        return action_succeeded;
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

        while (!m_board.getWinCoord().has_value()) {
            renderGameState();

            if (m_red_timer->hasTimeExpired() || m_blue_timer->hasTimeExpired()) {
                system("cls");
                std::cout << (m_red_timer->hasTimeExpired() ?
                    "RED player's time has expired! BLUE wins!\n" :
                    "BLUE player's time has expired! RED wins!\n");
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

        if (m_board.getWinCoord().has_value()) {
            std::cout << (m_curr_player.get().getColor() == color::ColorType::BLUE ?
                "Player RED has won!\n" : "Player BLUE has won!\n");
        }

        std::cin.get();
        std::cin.get();
    }

    void TimedMode::render() {
        renderGameState();
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

    void TimedMode::displayTimeSelection() {
        system("cls");
        std::cout << "\nSelect time limit per round (seconds):\n";
        std::cout << "1. 60 seconds\n";
        std::cout << "2. 90 seconds\n";
        std::cout << "3. 120 seconds\n";
        std::cout << "Choose an option (1-3): ";
    }
}