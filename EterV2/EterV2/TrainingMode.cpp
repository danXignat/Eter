#include "TrainingMode.h"

#include <format>

#include "logger.h"

using namespace logger;

namespace base {
	//---------------------------------------Constructor-------------------------------------
	TrainingMode::TrainingMode(bool illusion, bool explosion, const std::string& player1_name, const std::string& player2_name)
		: m_board{ 3 },
		m_win_manager{ m_board }
	{	
		m_player1.emplace(player1_name, color::ColorType::RED);
		m_player2.emplace(player2_name, color::ColorType::BLUE );

		if (illusion) {
			m_illusion_service.emplace(m_board);
		}

		if (explosion) {
			//m_explosion_service.emplace();
		}
	}

	//---------------------------------------Events----------------------------------------------

	void TrainingMode::run() {
		m_board.renderBoard();

		while (m_win_manager.won() == false) {
			InputHandler input;
			try {
				input.read();								//asta da handle la inputu de la tastatura
			}
			catch (const std::runtime_error& err) {
				Logger::log(Level::ERROR, err.what());
				continue;
			}
			
			auto card = _currPlayer().getCard(input.card_type.value());
			Coord coord { input.x.value(), input.y.value()};

			if (card.has_value()) {
				m_board.appendMove(coord, std::move(*card));

				m_win_manager.addCard(coord);

				_switchPlayer();
			}
			else {
				Logger::log(Level::WARNING, "No more cards of this type");
			}

			system("cls");
			m_board.renderBoard();
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

	
}