#include "TournamentMode.h"
#include "logger.h"
#include "MageMode.h"
#include "ElementalMode.h"

using namespace logger;

namespace base {

    TournamentMode::TournamentMode(const std::vector<ServiceType>& services,
        const std::pair<std::string, std::string>& player_names,
        std::unique_ptr<BaseGameMode> base_mode)
        : BaseGameMode(base_mode ? base_mode->getBoard().getSize() == 3 ? GameSizeType::SMALL : GameSizeType::BIG
            : GameSizeType::SMALL, player_names, services),
        m_arena_service(m_board.getSize() == 3 ? GameSizeType::SMALL : GameSizeType::BIG,
            m_player_red, m_player_blue),
        m_base_mode(std::move(base_mode)) {
    }

    void TournamentMode::displayModeSelection() {
        system("cls");
        std::cout << " Select tournament Mode:\n";
        std::cout << "1. Training Mode\n";
        std::cout << "2. Mage Mode\n";
        std::cout << "3. Elemental Mode\n";
        std::cout << " 4. Mage + Elemental Mode\n";
    }

    void TournamentMode::run() {
        if (!m_base_mode) {
            displayModeSelection();
            uint16_t choice;
            std::cin >> choice;
        }

        while (!m_tournament_ended) {
            system("cls");
            render();
        }
        InputHandler input;
        try {
            input.read();
            bool action_succeded = processGameInput(input);
            if (action_succeded) {
                switchPlayer();
            }
        }
        catch (const std::runtime_error& err) {
            Logger::log(Level::ERROR, err.what());
        }
    }

    void TournamentMode::render() {
        m_board.render();
        m_board.sideViewRender();
        m_arena_service.renderArena(m_board);

        if (auto* mageMode = dynamic_cast<MageMode*>(m_base_mode.get())) {
            mageMode->render();
        }
        else if (auto* elementalMode = dynamic_cast<ElementalMode*>(m_base_mode.get())) {
            elementalMode->render();
        }
    }

    bool TournamentMode::processGameInput(const InputHandler& input) {
        if (m_base_mode) {
            switch (input.event_type) {
            case EventType::PlaceCombatCard: {
                bool succes = placeCombatCard(input);
                if (succes && m_board.getWinCoord().has_value()) {
                    color::ColorType winner_color = getCurrPlayer().getColor();
                    m_arena_service.placeMarker(m_board.getWinCoord().value(), m_board, winner_color);
                }
                return succes;
            }
            }
            return false;
        }
    }
}