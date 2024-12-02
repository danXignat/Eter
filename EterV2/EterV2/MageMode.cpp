#include "MageMode.h"

#include <format>

#include "InputHandler.h"
#include "logger.h"

using namespace logger;

namespace base {
	//---------------------------------------Constructor-------------------------------------
	MageMode::MageMode(const std::vector<ServiceType>& services, const std::pair<std::string, std::string>& player_names) :
		BaseGameMode{GameSizeType::BIG, player_names},
		m_mage_service{ m_board }
	{

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

	void MageMode::run() {
		m_mage_service.selectMages();
		system("cls");

		this->render();
		while (m_win_manager.won() == false) {}
		//	InputHandler input;
		//	try {
		//		input.read();								//asta da handle la inputu de la tastatura
		//	}
		//	catch (const std::runtime_error& err) {
		//		Logger::log(Level::ERROR, err.what());
		//		system("cls");
		//		this->render();
		//		continue;
		//	}

		//	if (m_explosion_service) {
		//		bool is_explosion = input.service_type && input.service_type == ServiceType::EXPLOSION;
		//		if (is_explosion) {
		//			m_explosion_service->setting();
		//			m_explosion_service->apply();
		//			system("cls");
		//			this->render();
		//			continue;
		//		}
		//	}
		//	if (input.service_type.has_value() && input.service_type == ServiceType::MAGE) {
		//		m_mage_service.apply(m_curr_player.get());
		//		std::cin.get();
		//		_switchPlayer();
		//	}
		//	else if (m_curr_player.get().hasCard(input.card_type.value())){

		//		Coord coord{ input.x.value(), input.y.value() };
		//		auto type = input.card_type.value();
		//		CombatCard combat_card(type, m_curr_player.get().getColor());

		//		if (m_board.isValidMove(coord, combat_card)) {
		//			auto card = m_curr_player.get().getCard(input.card_type.value());

		//			if (m_illusion_service) {
		//				bool is_illusion = input.service_type && input.service_type == ServiceType::ILLUSION;
		//				if (is_illusion) {
		//					m_illusion_service->add(card.value());
		//				}
		//			}
		//			m_board.appendMove(coord, std::move(*card));
		//			m_win_manager.addCard(coord);
		//			_switchPlayer();
		//		}
		//		
		//	}
		//	else {
		//		Logger::log(Level::WARNING, "No more cards of this type");
		//	}

		//	system("cls");
		//	this->render();
		//}

		//if (m_curr_player.get().getColor() == color::ColorType::BLUE) {
		//	std::cout << "Player RED has won";
		//}
		//else {
		//	std::cout << "Player BLUE has won";
		//}

		//std::cin.get();
	}

	void MageMode::_handleSpecialEvent(const InputHandler& input) {

	}

	////------------------------------------------------Methods-------------------------------------------------

	void MageMode::render() {
		m_board.render();
		m_board.sideViewRender();
		m_player_red.renderCards();
		m_player_blue.renderCards();
		if (m_explosion_service) {
			m_explosion_service->render_explosion();
		}
	}

}