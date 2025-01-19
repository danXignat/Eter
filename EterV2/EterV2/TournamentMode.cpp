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
        if (auto win_coord = m_base_mode->getBoard().getWinCoord()) {
            color::ColorType winner_color = m_base_mode->getCurrPlayer().getColor();
            m_arena_service.placeMarker(win_coord.value(), m_base_mode->getBoard(), winner_color);

            if (winner_color == color::ColorType::RED) {
                m_red_wins++;
            }
            else {
                m_blue_wins++;
            }
            m_games_played++;

            if (m_arena_service.hasWinner()) {
                m_tournament_ended = true;
                std::string winner = (winner_color == color::ColorType::RED) ? m_player_names.first : m_player_names.second;
                std::cout << "Tournament won by " << winner << " by creating a line in arena!\n";
                return;
            }

            if (m_games_played >= MAX_GAMES) {
                m_tournament_ended = true;
                determineWinner();
            }
        }
    }

    void TournamentMode::determineWinner() {
        uint16_t red_markers = m_arena_service.countMarkers(color::ColorType::RED);
        uint16_t blue_markers = m_arena_service.countMarkers(color::ColorType::BLUE);
        std::cout << "Final arena markers count:\n";
        std::cout << m_player_names.first << " (Red): " << red_markers << "\n";
        std::cout << m_player_names.second << " (Blue): " << blue_markers << "\n";

        if (red_markers > blue_markers) {
            std::cout << m_player_names.first << " wins the tournament!\n";
        }
        else if (blue_markers > red_markers) {
            std::cout << m_player_names.second << " wins the tournament!\n";
        }
        else {
            std::cout << "Tournament ended in a draw!\n";
        }
    }

    void TournamentMode::updateScores() {
        std::cout << "Games played: " << m_games_played << "/" << MAX_GAMES << "\n";
        std::cout << m_player_names.first << " (Red) wins: " << m_red_wins << "\n";
        std::cout << m_player_names.second << " (Blue) wins: " << m_blue_wins << "\n";

        uint16_t red_markers = m_arena_service.countMarkers(color::ColorType::RED);
        uint16_t blue_markers = m_arena_service.countMarkers(color::ColorType::BLUE);
        std::cout << "Arena markers - Red: " << red_markers << ", Blue: " << blue_markers << "\n";
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
                m_base_mode = std::make_unique<MageMode>(services, m_player_names);
                m_elemental_mode = std::make_unique<ElementalMode>(services, m_player_names);
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

            InputHandler input;
            try {
                input.read();
                bool action_succeeded = processGameInput(input);
                if (action_succeeded) {
                    if (m_base_mode->getBoard().getWinCoord().has_value()) {
                        handleGameEnd();
                        updateScores();
                    }
                    m_base_mode->switchPlayer();
                }
            }
            catch (const std::runtime_error& err) {
                Logger::log(Level::ERROR, err.what());
            }
        }
    }

    void TournamentMode::render() {
        Board& board = const_cast<Board&>(m_base_mode->getBoard());
        board.render();
        board.sideViewRender();

        m_base_mode->getPlayerRed().renderCards();
        m_base_mode->getPlayerBlue().renderCards();

        m_arena_service.renderArena(board);

        if (auto* mageMode = dynamic_cast<MageMode*>(m_base_mode.get())) {
            mageMode->render();
            m_arena_service.renderArena(board);
        }
        else if (auto* elementalMode = dynamic_cast<ElementalMode*>(m_base_mode.get())) {
            elementalMode->render();
            m_arena_service.renderArena(board);
        }

        if (m_explosion_service) {
            m_explosion_service->render();
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
                success = mageMode->useMage();
            }
            break;
        }
        case EventType::UsePower: {
            if (auto* elementalMode = dynamic_cast<ElementalMode*>(m_base_mode.get())) {
                success = elementalMode->usePower();
            }
            break;
        }
        default:
            break;
        }
        return success;
    }
}
