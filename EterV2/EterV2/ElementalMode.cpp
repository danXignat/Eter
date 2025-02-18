#include "ElementalMode.h"


#include <format>

#include "InputHandler.h"
#include "logger.h"

using namespace logger;

namespace base {
	//---------------------------------------Constructor-------------------------------------
	ElementalMode::ElementalMode(const std::vector<ServiceType>& services, const std::pair<std::string, std::string>& player_names) :
		BaseGameMode{ GameSizeType::BIG, player_names, services},
		m_elemental_service{ m_board,m_player_red, m_player_blue, m_illusion_service, m_explosion_service}
	{
	}

	//---------------------------------------Events----------------------------------------------

	void ElementalMode::run() {
		this->render();

		while (true) {
			InputHandler input;
			try {
				input.read();								//asta da handle la inputu de la tastatura
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
				if (m_board.isRowBlocked(input.coord.second, m_curr_player.get().getColor()) || 
					(m_board.isColumnBlocked(input.coord.first, m_curr_player.get().getColor()))) {
					Logger::log(Level::WARNING, "This row is blocked by Blizzard");
					action_succed=false;
					m_board.clearBlockedRow();
				}
				else {
					action_succed = placeCombatCard(input);
				}
				break;

			case PlaceIllusion:
				//action_succed = placeIllusion(input);
				break;

			case UsePower:
				//action_succed = usePower();
				break;

			default:
				break;
			}

			/*if (isExplosionAvailable()) {
				this->render();
				setExplosion();
				detonateExplosion();
			}*/

			if (action_succed) {
				switchPlayer();
				this->render();
			}
		}

		if (m_curr_player.get().getColor() == color::ColorType::BLUE) {
			std::cout << "Player RED has won";
		}
		else {
			std::cout << "Player BLUE has won";
		}

		std::cin.get();
	}
	
	////------------------------------------------------Methods-------------------------------------------------

	void ElementalMode::render() {
		system("cls");

		m_board.render();
		m_board.sideViewRender();
		m_player_red.renderCards();
		m_player_blue.renderCards();
		m_elemental_service.renderCards();

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
			m_explosion_service->render();
		}
	}

	ElementalService& ElementalMode::getElementalService() {
		return m_elemental_service;
	}
}