#include "ElementalMode.h"


#include <format>

#include "InputHandler.h"
#include "logger.h"

using namespace logger;

namespace base {
	//---------------------------------------Constructor-------------------------------------
	ElementalMode::ElementalMode(const std::vector<ServiceType>& services, const std::string& player1_name, const std::string& player2_name)
		: m_board{ 3 },
		m_win_manager{ m_board },
		m_player_red{ player1_name, color::ColorType::RED },
		m_player_blue{ player2_name, color::ColorType::BLUE },
		curr_player{ m_player_red },
		m_elemental_service{ m_board } {

		for (ServiceType service : services) {
			switch (service) {
				using enum ServiceType;
			case ILLUSION:
				m_illusion_service.emplace(m_board);
				break;
			case EXPLOSION:
				m_explosion_service.emplace(m_board, m_player_red, m_player_blue);
				break;
			default:
				break;
			}
		}
	}

	//---------------------------------------Events----------------------------------------------

	void ElementalMode::run() {
		this->render();

		while (m_win_manager.won() == false) {
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

			if (m_explosion_service) {
				bool is_explosion = input.service_type && input.service_type == ServiceType::EXPLOSION;
				if (is_explosion) {
					m_explosion_service->setting();
					m_explosion_service->apply();
					system("cls");
					this->render();
					continue;
				}
			}
			if (input.service_type.has_value() && input.service_type == ServiceType::ELEMENTAL) {
				m_elemental_service.apply(curr_player.get());
				std::cin.get();
			}
			else if (auto card = curr_player.get().getCard(input.card_type.value())) {
				Coord coord{ input.x.value(), input.y.value() };

				if (m_illusion_service) {
					bool is_illusion = input.service_type && input.service_type == ServiceType::ILLUSION;
					if (is_illusion) {
						m_illusion_service->add(card.value());
					}
				}

				m_board.appendMove(coord, std::move(*card));

				m_win_manager.addCard(coord);

				switchPlayer();
			}
			else {
				Logger::log(Level::WARNING, "No more cards of this type");
			}

			system("cls");
			this->render();
		}

		if (curr_player.get().getColor() == color::ColorType::BLUE) {
			std::cout << "Player RED has won";
		}
		else {
			std::cout << "Player RED has won";
		}

		std::cin.get();
	}

	////------------------------------------------------Methods-------------------------------------------------

	void ElementalMode::switchPlayer() {
		if (curr_player.get().getColor() == color::ColorType::RED) {
			curr_player = m_player_blue;
		}
		else {
			curr_player = m_player_red;
		}
	}

	void ElementalMode::render() {
		m_board.render();

		if (m_explosion_service) {
			m_explosion_service->render_explosion();
		}
	}

}