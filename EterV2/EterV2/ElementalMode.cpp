#include "ElementalMode.h"


#include "TrainingMode.h"


#include <format>

#include "logger.h"

using namespace logger;

namespace base {
	//---------------------------------------Constructor-------------------------------------
	ElementalMode::ElementalMode(bool illusion, bool explosion, const std::string& player1_name, const std::string& player2_name)
		: m_board{ 3 },
		m_win_manager{ m_board },
		m_elemental_service{ m_board }
	{
		m_player1.emplace(player1_name, color::ColorType::RED);
		m_player2.emplace(player2_name, color::ColorType::BLUE);

		if (illusion) {
			m_illusion_service.emplace(m_board);
		}

		if (explosion) {
			m_explosion_service.emplace(m_board, m_player1.value(), m_player2.value());
		}
	}

	//---------------------------------------Events----------------------------------------------

	void ElementalMode::run() {
		m_elemental_service.selectPowerCards();
		system("cls");

		this->render();

		while (m_win_manager.won() == false) {
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

			if (input.service_type.has_value() && input.service_type == ServiceType::ELEMENTAL) {
				m_elemental_service.apply(_currPlayer());
				std::cin.get();
			}
			else if (auto card = _currPlayer().getCard(input.card_type.value())) {

				Coord coord{ input.x.value(), input.y.value() };

				m_board.appendMove(coord, std::move(*card));

				m_win_manager.addCard(coord);

				_switchPlayer();
			}
			else {
				Logger::log(Level::WARNING, "No more cards of this type");
			}

			system("cls");
			this->render();
		}

		if (_currPlayer().getColor() == color::ColorType::BLUE) {
			std::cout << "Player RED has won";
		}
		else {
			std::cout << "Player RED has won";
		}

		std::cin.get();
	}

	////------------------------------------------------Methods-------------------------------------------------

	void ElementalMode::render() {
		m_board.render();

		if (m_explosion_service) {
			m_explosion_service->render_explosion();
		}
	}

}