#include "TrainingMode.h"

#include <format>

#include "InputHandler.h"
#include "logger.h"

using namespace logger;

namespace base {
	//---------------------------------------Constructor-------------------------------------
	TrainingMode::TrainingMode(const std::vector<ServiceType>& services, const std::pair<std::string, std::string>& player_names) :
		BaseGameMode{ GameSizeType::SMALL, player_names, services }
	{}

	//---------------------------------------Events----------------------------------------------

	void TrainingMode::run() {
		this->render();

		while (m_board.getWinCoord().has_value() == false) {
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

	void TrainingMode::render() {
		system("cls");

		m_board.render();
		m_board.sideViewRender();
		m_player_red.renderCards();
		m_player_blue.renderCards();

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
	
}