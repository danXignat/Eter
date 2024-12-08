#include "InputHandler.h"

#include <string>
#include <iostream>
#include <sstream>
#include <regex>

namespace base {
	InputHandler::InputHandler() :
		coord{0, 0},
		card_type{ CombatCardType::NONE },
		service_type{ ServiceType::NONE },
		event_type{ GameEventType::NONE } {

	}

	void InputHandler::read() {
		std::string input;
		std::getline(std::cin, input);

		std::istringstream stream{ input };
		std::vector<std::string> tokens;
		std::string token;

		while (stream >> token) {
			tokens.push_back(token);
		}

		if (tokens.size() < 1) {
			throw std::runtime_error("No arguments");
		}
		else if (tokens.size() == 1) {
			std::string special_card{ tokens.front() };
			std::regex pattern{ R"(^[emp]$)" };

			if (special_card.size() == 1 && std::regex_match(special_card, pattern)) {
				service_type = charToService(special_card.front());
			}
			else {
				throw std::runtime_error("Invalid special card");
			}

			event_type = GameEventType::SpecialCard;
		}
		else if (tokens.size() > 1 && tokens.size() <= 3) {
			std::regex coord_pattern{ R"(^\d{0,2}$)" };
			std::regex card_pattern{ R"(^[1234E]$)" };

			std::string pos1{ tokens[0] }, pos2{ tokens[1] };
			std::string card_type_str{ tokens[2] };

			if (!std::regex_match(pos1, coord_pattern)) {
				throw std::runtime_error("Invalid x-coordinate");
			}
			coord.first = std::stoi(pos1);

			if (!std::regex_match(pos2, coord_pattern)) {
				throw std::runtime_error("Invalid y-coordinate");
			}
			coord.second = std::stoi(pos2);

			if (!std::regex_match(card_type_str, card_pattern)) {
				throw std::runtime_error("Invalid card choice");
			}
			card_type = charToCombatCard(card_type_str.front());

			event_type = GameEventType::BoardCard;
		}
		else if (tokens.size() > 1 && tokens.size() <= 4) {
			std::regex coord_pattern{ R"(^\d{0,2}$)" };
			std::regex card_pattern{ R"(^[1234E]$)" };

			std::string pos1{ tokens[0] }, pos2{ tokens[1] };
			std::string card_type_str{ tokens[2] };

			if (!std::regex_match(pos1, coord_pattern)) {
				throw std::runtime_error("Invalid x-coordinate");
			}
			coord.first = std::stoi(pos1);

			if (!std::regex_match(pos2, coord_pattern)) {
				throw std::runtime_error("Invalid y-coordinate");
			}
			coord.second = std::stoi(pos2);

			if (!std::regex_match(card_type_str, card_pattern)) {
				throw std::runtime_error("Invalid card choice");
			}
			card_type = charToCombatCard(card_type_str.front());

			if (tokens.size() == 4) {
				if (tokens[3].front() != 'i') {
					throw std::runtime_error("Invalid service choice");
				}
				service_type = charToService(tokens[3].front());
			}

			event_type = GameEventType::SpecialCard;
		}
		else {
			throw std::runtime_error("Invalid argument number");
		}
		
	}
}