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
        m_arena_service{ GameSizeType::BIG, m_board, m_player_red, m_player_blue } {

        for (ServiceType service : services) {
            switch (service) {
            case base::ServiceType::MAGE:
                m_mage_service.emplace(m_board, m_player_red, m_player_blue);
                break;

            case base::ServiceType::ELEMENTAL:
                m_elemental_service.emplace(m_board, m_player_red, m_player_blue, m_illusion_service, m_explosion_service);
                break;

            default:
                break;
            }
        }
    }

    void TournamentMode::run() {
        /*if (!m_base_mode) {
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
        }*/
    }

    void TournamentMode::render() {
        /*Board& board = const_cast<Board&>(m_base_mode->getBoard());
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
        }*/
    }

    std::optional<ElementalService>& TournamentMode::getElementalService() {
        return m_elemental_service;
    }

    std::optional<MageService>& TournamentMode::getMageService() {
        return m_mage_service;
    }

    ArenaService& TournamentMode::getArenaService() {
        return m_arena_service;
    }

    void TournamentMode::nextRound() {
        auto win_data{ m_board.getWinData() };

        m_arena_service.addMarker(win_data->first, win_data->second);

        BaseGameMode::nextRound();
    }
}
