#include "GameMode.h"

#include <unordered_set>

namespace base {

	GameMode::GameMode() : m_current_player{ 0 } {

	}

	//--------------------------------------------------Methods-----------------------------------

	void GameMode::_switchPlayer() {
		m_current_player = !m_current_player;
	}

	Player& GameMode::_curentPlayer() {
		return (m_current_player == 0) ? m_player1.value() : m_player2.value();
	}

	CombatCardType GameMode::_fromCharToType(char ch) {
		using enum CombatCardType;

		std::unordered_map<char, CombatCardType> switch_map{
			{'1', ONE},
			{'2', TWO},
			{'3', THREE},
			{'4', FOUR},
			{'E', ETER}
		};

		return switch_map[ch];
	}

	////----------------------------------------------Win Manager-----------------------------------------------

	GameMode::WinManager::WinManager(uint16_t size, Board& board) 
		: size{ size },
		board{ board } {

	}

	bool GameMode::WinManager::won(const Coord& coord, const Player& player) {
		const auto& [x, y] = coord;

		int16_t increment = (player.getColor() == color::ColorType::BLUE) ? 1 : -1;

		rows[y] += increment;
		cols[x] += increment;

		bool init_diags = (diag1 && diag2);
		if (board.isFixed() && init_diags == false) {
			diag1.emplace(0);
			diag2.emplace(0);

			for (uint16_t i = 0; i < size; i++) {
				Coord corner = board.getLeftCorner();

				Coord pos1{ corner.first + 2 * i, corner.second + i };
				auto card = board.getTopCard(pos1);
				if (card != std::nullopt) {
					increment = (card->get().getColor() == color::ColorType::BLUE) ? 1 : -1;

					diag1.value() += increment;
				}

				Coord pos2{ corner.first + 2 * (size - 1 - i), corner.second + i};
				card = board.getTopCard(pos2);
				if (card != std::nullopt) {
					increment = (card->get().getColor() == color::ColorType::BLUE) ? 1 : -1;

					diag2.value() += increment;
				}
			}

			init_diags = true;
		}

		if (init_diags) {
			if (abs(diag1.value()) == size || abs(diag2.value()) == size) {
				return true;
			}
		}

		if (abs(rows[y]) == size || abs(cols[x]) == size) {
			return true;
		}

		return false;
	}

	//-------------------------------------------Input Manager---------------------------------------------

	void GameMode::Input::read() {
		std::unordered_set<char> available_moves{ '1', '2', '3', '4' };

		std::string input;
		std::getline(std::cin, input);

		std::istringstream stream{ input };
		std::string token;
		char del{ ' ' };

		std::getline(stream, token, del);

		std::cout << token;
		if (utils::isValidPos(token)) {
			x = std::stoi(token);
		}
		else {
			throw std::invalid_argument("number is needed");
		}

		std::getline(stream, token, del);
		if (utils::isValidPos(token)) {
			y = std::stoi(token);
		}
		else {
			throw std::invalid_argument("number is needed");
		}

		std::getline(stream, token, del);
		if (token.size() == 1 && available_moves.contains(token[0])) {
			card_type = token[0];
		}
		else {
			throw std::invalid_argument("not valid choice");
		}

		std::getline(stream, token, del);
		if (!token.empty() && token.size() == 1 && token[0] == 'i') {
			illusion = true;
		}
		else {
			illusion = false;
		}
	}
}
