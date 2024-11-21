#include "GameMode.h"

#include <unordered_set>

namespace base {

	GameMode::GameMode() : m_current_player{ 0 } {

	}

	//--------------------------------------------------Methods-----------------------------------

	void GameMode::_switchPlayer() {
		m_current_player = !m_current_player;
	}

	Player& GameMode::_currPlayer() {
		return (m_current_player == 0) ? m_player1.value() : m_player2.value();
	}

	//-------------------------------------------Input Manager---------------------------------------------

	void GameMode::InputHandler::read() {
		std::string input;
		std::getline(std::cin, input);

		std::istringstream stream{ input };
		std::string token;
		char del{ ' ' };

		std::regex coord_pattern{ R"(^\d{0,2}\n?$)" };
		std::regex card_pattern{ R"(^1|2|3|4|E\n?$)" };
		std::regex service_pattern{ R"(^i|e|m|p\n?$)" };

		if (!std::getline(stream, token, del) || !std::regex_match(token, coord_pattern)) {
			std::cout << std::regex_match(token, coord_pattern) << " " << std::stoi(token);
			throw std::runtime_error("Invalid x-coordinate");
		}
		x = std::stoi(token);

		if (!std::getline(stream, token, del) || !std::regex_match(token, coord_pattern)) {
			throw std::runtime_error("Invalid y-coordinate");
		}
		y = std::stoi(token);

		if (!std::getline(stream, token, del) || !std::regex_match(token, card_pattern)) {
			throw std::runtime_error("Invalid card choice");
		}
		card_type = charToCombatCard(token.front());

		if (std::getline(stream, token, del) && !std::regex_match(token, service_pattern)) {
			throw std::runtime_error("Invalid service choice");
		}
		service_type = charToService(token.front());
	}
}
