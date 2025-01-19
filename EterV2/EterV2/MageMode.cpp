#include "MageMode.h"

#include <format>

#include "InputHandler.h"
#include "logger.h"

using namespace logger;

namespace base {
	//---------------------------------------Constructor-------------------------------------
	MageMode::MageMode(const std::vector<ServiceType>& services, const std::pair<std::string, std::string>& player_names) :
		BaseGameMode{GameSizeType::BIG, player_names, services},
		m_mage_service{ m_board }
	{}

	//---------------------------------------Events----------------------------------------------

	void MageMode::run() {
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

			bool action_succed;
			switch (input.event_type) {
				using enum EventType;

			case PlaceCombatCard:
				action_succed = placeCombatCard(input);
				break;

			case PlaceIllusion:
				action_succed = placeIllusion(input);
				break;

			case UseMage:
				action_succed = useMage();
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

	bool MageMode::useMage() {
		return m_mage_service.apply(m_curr_player.get());
	}

	////------------------------------------------------Methods-------------------------------------------------

	void MageMode::render() {
		system("cls");

		m_board.render();
		m_board.sideViewRender();
		m_player_red.renderCards();
		m_player_blue.renderCards();
		m_mage_service.renderCard(m_curr_player);

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

}