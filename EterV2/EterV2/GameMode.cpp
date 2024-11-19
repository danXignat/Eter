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

	GameMode::WinManager::WinManager(Board& board) 
		:	board{ board },
			board_size{ board.size() },
			diag1{ 0 },
			diag2{ 0 },
			are_diags_init{ false }
	{}

	bool GameMode::WinManager::addCardAndCheck(const Coord& coord) {
		const auto& [x, y] = coord;
		int16_t increment = _getIncrement(coord);

		rows[y] += increment;
		cols[x] += increment;

		if (board.isFixed() && are_diags_init == false) {
			_setDiags();
		}
		else if (are_diags_init == true) {
			const auto& [corner_x, corner_y] = board.getLeftCorner();

			uint16_t local_x = (x - corner_x) / 2;
			uint16_t local_y = y - corner_y;

			if (local_x == local_y) {
				diag1 += increment;
			}

			if (local_x + local_y == board_size - 1) {
				diag2 += increment;
			}
		}

		return won(coord);
	}

	bool GameMode::WinManager::won(const Coord& coord) {
		const auto& [x, y] = coord;

		if (abs(rows[y]) == board_size || abs(cols[x]) == board_size) {
			return true;
		}

		if (abs(diag1) == board_size || abs(diag2) == board_size) {
			return true;
		}

		return false;
	}

	int16_t GameMode::WinManager::_getIncrement(const Coord& coord) {
		int16_t increment = 0;

		if (auto card = board.getTopCard(coord)) {
			increment = (card->get().getColor() == color::ColorType::BLUE) ? 1 : -1;
		}

		return increment;
	}

	void GameMode::WinManager::_setDiags() {
		for (uint16_t i = 0; i < board_size; i++) {
			const auto& [x, y] = board.getLeftCorner();

			Coord pos1{ x + 2 * i, y + i };
			Coord pos2{ x + 2 * (board_size - 1 - i), y + i };

			diag1 += _getIncrement(pos1);
			diag2 += _getIncrement(pos2);
		}

		are_diags_init = true;
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
