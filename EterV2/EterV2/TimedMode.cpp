#include "TimedMode.h"
#include "InputHandler.h"
#include "logger.h"

using namespace logger;

namespace base {
    TimedMode::TimedMode(const std::vector<ServiceType>& services,
        const std::pair<std::string, std::string>& player_names,
        int time_limit_seconds) :
        BaseGameMode{ GameSizeType::SMALL, player_names, services },
        time_limit(time_limit_seconds),
        red_timer(std::make_unique<Timer>(time_limit_seconds)),
        blue_timer(std::make_unique<Timer>(time_limit_seconds))
    {
        red_timer->start(); 
    }

    void TimedMode::switchTimers() {
        if (m_curr_player.get().getColor() == color::ColorType::RED) {
            red_timer->stop();
            blue_timer->start();
        }
        else {
            blue_timer->stop();
            red_timer->start();
        }
    }

    void TimedMode::renderTimers() {
        utils::printAtCoordinate("Time remaining:", { 1, 14 });

        std::cout << color::to_string(color::ColorType::RED);
        utils::printAtCoordinate("RED: " + std::to_string(red_timer->getRemainingTime()) + "s", { 1, 15 });
        std::cout << color::to_string(color::ColorType::DEFAULT);

        std::cout << color::to_string(color::ColorType::BLUE);
        utils::printAtCoordinate("BLUE: " + std::to_string(blue_timer->getRemainingTime()) + "s", { 1, 16 });
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
        this->render();

        while (m_board.getWinCoord().has_value() == false) {
            
            bool red_expired = red_timer->hasTimeExpired();
            bool blue_expired = blue_timer->hasTimeExpired();

            if (red_expired || blue_expired) {
                system("cls");
                if (red_expired) {
                    std::cout << "RED player's time has expired! BLUE wins!\n";
                }
                else {
                    std::cout << "BLUE player's time has expired! RED wins!\n";
                }
                std::cin.get();
                return;
            }

            InputHandler input;
            try {
                input.read();
            }
            catch (const std::runtime_error& err) {
                Logger::log(Level::ERROR, err.what());
                system("cls");
                this->render();
                continue;
            }

            bool action_succed = false;
            switch (input.event_type) {
                using enum EventType;

            case PlaceCombatCard:
                action_succed = placeCombatCard(input);
                break;

            case PlaceIllusion:
                action_succed = placeIllusion(input);
                break;

            default:
                break;
            }

            /*if (isExplosionAvailable()) {
                this->render();
                setExplosion();
                detonateExplosion();
                m_explosion_service.reset();
            }*/

            if (action_succed) {
                switchTimers();  
                switchPlayer();
                this->render();
            }
        }

        //opreste timer
        red_timer->stop();
        blue_timer->stop();

        if (m_curr_player.get().getColor() == color::ColorType::BLUE) {
            std::cout << "Player RED has won";
        }
        else {
            std::cout << "Player BLUE has won";
        }

        std::cin.get();
    }
}