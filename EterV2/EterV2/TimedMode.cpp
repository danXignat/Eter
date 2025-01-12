#include <thread>
#include <conio.h>
#include "TimedMode.h"
#include "InputHandler.h"
#include "logger.h"

using namespace logger;

namespace base {
    TimedMode::TimedMode(const std::vector<ServiceType>& services,
        const std::pair<std::string, std::string>& player_names,
        int time_limit_seconds)
        : BaseGameMode{ GameSizeType::SMALL, player_names, services },
        time_limit(time_limit_seconds),
        red_timer(std::make_unique<Timer>(time_limit_seconds)),
        blue_timer(std::make_unique<Timer>(time_limit_seconds)) {
        red_timer->start();
    }

    void TimedMode::switchTimers() {
        if (m_curr_player.get().getColor() == color::ColorType::RED) {
            red_timer->stop();
            blue_timer->reset();
            blue_timer->start();
        }
        else {
            blue_timer->stop();
            red_timer->reset();
            red_timer->start();
        }
    }

    void TimedMode::renderTimers() {
        // Mutăm afișarea timpului mai sus, pentru claritate
        utils::printAtCoordinate("Time remaining:", { 50, 2 });
        std::cout << color::to_string(color::ColorType::RED);
        utils::printAtCoordinate("RED: " + std::to_string(red_timer->getRemainingTime()) + "s", { 50, 3 });
        std::cout << color::to_string(color::ColorType::DEFAULT);
        std::cout << color::to_string(color::ColorType::BLUE);
        utils::printAtCoordinate("BLUE: " + std::to_string(blue_timer->getRemainingTime()) + "s", { 50, 4 });
        std::cout << color::to_string(color::ColorType::DEFAULT);
    }


    void TimedMode::render() {
        system("cls");
        m_board.render();
        m_board.sideViewRender();
        m_player_red.renderCards();
        m_player_blue.renderCards();
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

    void TimedMode::run() {
        // Selectarea limitei de timp
        std::cout << "Select time limit per round (seconds):\n1. 60 seconds\n2. 90 seconds\n3. 120 seconds\n";
        int choice;
        std::cin >> choice;

        switch (choice) {
        case 1:
            time_limit = 60;
            break;
        case 2:
            time_limit = 90;
            break;
        case 3:
            time_limit = 120;
            break;
        default:
            std::cout << "Invalid choice, defaulting to 60 seconds.\n";
            time_limit = 60;
            break;
        }

        // Resetăm și pornim timer-ul pentru RED
        red_timer->reset();
        blue_timer->reset();
        red_timer->start();

        // Bucla principală a jocului
        while (!m_board.getWinCoord().has_value()) {
            // Actualizăm timer-ul în timp real
            system("cls");
            this->render();
            renderTimers();

            // Verificăm dacă timpul a expirat
            if (red_timer->hasTimeExpired() || blue_timer->hasTimeExpired()) {
                system("cls");
                if (red_timer->hasTimeExpired()) {
                    std::cout << "RED player's time has expired! BLUE wins!\n";
                }
                else {
                    std::cout << "BLUE player's time has expired! RED wins!\n";
                }
                std::cin.get();
                return;
            }

            // Așteptăm o secundă pentru actualizarea timpului
            std::this_thread::sleep_for(std::chrono::seconds(1));

            // Verificăm input-ul jucătorului non-blocant
            if (_kbhit()) {
                InputHandler input;
                try {
                    input.read();
                }
                catch (const std::runtime_error& err) {
                    Logger::log(Level::ERROR, err.what());
                    continue;
                }

                bool action_succeeded = false;
                switch (input.event_type) {
                    using enum EventType;
                case PlaceCombatCard:
                    action_succeeded = placeCombatCard(input);
                    break;
                case PlaceIllusion:
                    action_succeeded = placeIllusion(input);
                    break;
                default:
                    break;
                }

                if (action_succeeded) {
                    switchTimers();
                    switchPlayer();
                }
            }
        }

        // Oprirea timer-elor la final
        red_timer->stop();
        blue_timer->stop();

        if (m_curr_player.get().getColor() == color::ColorType::BLUE) {
            std::cout << "Player RED has won\n";
        }
        else {
            std::cout << "Player BLUE has won\n";
        }

        std::cin.get();
    }

}
