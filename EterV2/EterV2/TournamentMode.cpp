#include "TournamentMode.h"
#include "logger.h"
#include "MageMode.h"
#include "ElementalMode.h"
#include "TrainingMode.h"
#include "GameModeFactory.h"
using namespace logger;

namespace base {

    TournamentMode::TournamentMode(
        const std::vector<ServiceType>& services,
        const std::pair<std::string, std::string>& player_names)
        : BaseGameMode(GameSizeType::BIG, player_names, services),
        m_arena_service{ m_board.getSize() == 3 ? GameSizeType::SMALL : GameSizeType::BIG, m_player_red, m_player_blue },
        m_games_played{ 0 }, m_red_wins{ 0 }, m_blue_wins{ 0 }, m_tournament_ended{ false },
        m_player_names{ player_names } {
    }

    void TournamentMode::displayModeSelection() {
        system("cls");
        std::cout << " Select tournament Mode:\n";
        std::cout << "1. Training Mode\n";
        std::cout << "2. Mage Mode\n";
        std::cout << "3. Elemental Mode\n";
        std::cout << "4. Mage + Elemental Mode\n";
    }

    void TournamentMode::handleGameEnd() {
        if (m_board.getWinCoord().has_value()) {
            color::ColorType winner_color = getCurrPlayer().getColor();
            m_arena_service.placeMarker(m_board.getWinCoord().value(), m_board, winner_color);

            if (winner_color == color::ColorType::RED) {
                m_red_wins++;
            }
            else {
                m_blue_wins++;
            }
            m_games_played++;
        }
    }

    void TournamentMode::updateScores() {
        system("cls");

    }

    void TournamentMode::run() {
        if (!m_base_mode) {
            displayModeSelection();
            uint16_t choice;
            std::cin >> choice;
            std::string id = std::to_string(choice);
            auto services = GameModeFactory::getServiceConfig(id);
            switch (choice) {
            case 1:
                m_base_mode = std::make_unique<TrainingMode>(services, m_player_names);
                break;
            case 2:
                m_base_mode = std::make_unique<MageMode>(services, m_player_names);
                break;
            case 3:
                m_base_mode = std::make_unique<ElementalMode>(services, m_player_names);
                break;
            case 4:
            //m_mage_mode = MageMode(services, m_player_names);
            //m_elemental_mode = ElementalMode(services, m_player_names);
                break;
            default:
                std::cout << "Invalid choice, defaulting to Training Mode.\n";
                m_base_mode = std::make_unique<TrainingMode>(services, m_player_names);
                break;
            }
        }

        while (!m_tournament_ended) {
            system("cls");
            render();


            if (m_games_played >= MAX_GAMES) {
                m_tournament_ended = true;
                std::cout << "Tournament is over ";
            }

            InputHandler input;
            try {
                input.read();
                bool action_succeded = processGameInput(input);
                if (action_succeded) {
                    m_base_mode->switchPlayer();

                }
            }
            catch (const std::runtime_error& err) {
                Logger::log(Level::ERROR, err.what());
            }
            if (m_tournament_ended) {
                break;
            }
        }
    }

    void TournamentMode::render() {

        Board& board = const_cast<Board&>(m_base_mode->getBoard());
        board.render();
        board.sideViewRender();

        m_base_mode->getPlayerRed().renderCards();
        m_base_mode->getPlayerBlue().renderCards();

        m_arena_service.renderArena(m_board);

        if (auto* mageMode = dynamic_cast<MageMode*>(m_base_mode.get())) {
            mageMode->render();
        }
        else if (auto* elementalMode = dynamic_cast<ElementalMode*>(m_base_mode.get())) {
            elementalMode->render();
        }

        if (m_explosion_service) {
            m_explosion_service->renderExplosion();
        }
    }

    bool TournamentMode::processGameInput(const InputHandler& input) {
        bool success = false;
            switch (input.event_type) {
            case EventType::PlaceCombatCard: {
               success = m_base_mode->placeCombatCard(input);
                break;
            }

            case EventType::PlaceIllusion: {
               success = m_base_mode->placeIllusion(input);
                break;
            }
            case EventType::UseMage: {
                if (auto* mageMode = dynamic_cast<MageMode*>(m_base_mode.get())) {
                    success= mageMode->useMage();
                }
                break;
            }
            case EventType::UsePower: {
                if (auto* elementalMode = dynamic_cast<ElementalMode*>(m_base_mode.get())) {
                    success= elementalMode->usePower();
                }
                break;
            }
            default:
                break;
            }
        return success;
    }
}