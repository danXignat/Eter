#include "MageMode.h"


#include "TrainingMode.h"

#include <format>

#include "logger.h"

using namespace logger;

namespace base {
	//---------------------------------------Constructor-------------------------------------
	MageMode::MageMode(bool illusion, bool explosion, const std::string& player1_name, const std::string& player2_name)
		: m_board{ 3 },
		m_win_manager{ m_board },
		m_mage_service{ m_board }
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

	void MageMode::run() {
		m_mage_service.selectMages();
		system("cls");

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

			if (input.service_type.has_value() && input.service_type == ServiceType::MAGE) {
				m_mage_service.apply(_currPlayer());
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

	void MageMode::render() {
		m_board.render();

		if (m_explosion_service) {
			m_explosion_service->render_explosion();
		}
	}

}