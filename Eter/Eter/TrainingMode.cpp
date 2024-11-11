module TrainingMode;

import <iostream>;
import <memory>;
import <unordered_map>;
import <format>;

import CombatCard;
import CombatCardType;
import Board;
import GameModeTypes;
import utils;
import Teams;
import Logger;
import Player;

namespace base {
	//---------------------------------------Constructor-------------------------------------

	TrainingMode::TrainingMode(std::string player1_name, std::string player2_name) :
		m_player1{ player1_name, GameModeTypes::Training, teams::Team::RED },
		m_player2{ player2_name, GameModeTypes::Training, teams::Team::BLUE },
		m_board{ 3 },
		m_win_manager{ 3 } {

	}

	//---------------------------------------Events----------------------------------------------

	void TrainingMode::gameLoop() {

		bool win = false;
		m_board.renderBoard();

		while (!win) {
			uint16_t x, y;
			char card_type, is_illusion;
			std::cin >> x >> y >> card_type>>is_illusion;

			CombatCardType type = this->_fromCharToType(card_type);
			Player& current_player = this->_curentPlayer();

			bool illusion = (is_illusion == 'Y' || is_illusion == 'y');

			auto card = current_player.getCard(type); //optional cu cartea se extrage cartea din invetaru playerului

			if (card) {
				m_board.appendMove(
					{ x, y }, std::move(*card)
				);

				win = m_win_manager.won(
					{ x, y }, current_player, m_board
				);

				this->_switchPlayer();
			}
			else {
				using namespace logger;

				Logger::log(Level::WARNING, "no cards left of type '{}'", card_type);
			}

			system("cls");
			m_board.renderBoard();
		}

		std::cout << std::format("Player {}",
			(this->_curentPlayer().getTeam() == teams::Team::BLUE) ? "BLUE" : "RED"
		);
		std::cin.get();
	}

	//------------------------------------------------Methods-------------------------------------------------

	CombatCardType TrainingMode::_fromCharToType(char ch) {
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

	void TrainingMode::_switchPlayer() {
		m_current_player = !m_current_player;
	}

	Player& TrainingMode::_curentPlayer() {
		return (m_current_player) ? m_player1 : m_player2;
	}

	//------------------------------------------------Win Manager-----------------------------------------------

	TrainingMode::WinManager::WinManager(uint16_t size) : size{ size } {
		
	}

	bool TrainingMode::WinManager::won(const Coord& coord, const Player& player, const Board& board) {
		const auto& [x, y] = coord;

		int16_t incrementor = (player.getTeam() == teams::Team::BLUE) ? 1 : -1;

		rows[y] += incrementor;
		cols[x] += incrementor;

		if (abs(rows[y]) == size || abs(cols[x]) == size) {
			return true;
		}

		if (board.isFixed()) {
			//board.diagsCount() -> pair <nr elemente diag1, nr element diag2>
		}

		return false;
	}
}

