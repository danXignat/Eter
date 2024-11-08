module TrainingMode;

import <iostream>;
import <memory>;
import <unordered_map>;

import CombatCard;
import CombatCardType;
import Board;
import GameModeTypes;
import utils;

namespace base {
	//---------------------------------------------------------------Constructor-------------------------------------

	TrainingMode::TrainingMode(std::string player1_name, std::string player2_name) :
		m_player1{ player1_name, GameModeTypes::Training },
		m_player2{ player2_name, GameModeTypes::Training },
		m_board{ 3 } {
	}

	//---------------------------------------------------------------Events----------------------------------------------



	void TrainingMode::gameLoop(bool win) {
		using enum CombatCardType;

		while (!win) {
			system("cls");
			m_board.renderBoard();

			uint16_t posX, posY;
			char card_type;
			std::cin >> posX >> posY >> card_type;

			std::unordered_map<char, base::CombatCardType> switch_map{
				{'1', ONE},
				{'2', TWO},
				{'3', THREE},
				{'4', FOUR},
				{'E', ETER}
			};

			CombatCardType move_card_type = switch_map[card_type];

			auto& currentPlayerRef = currentPlayer ? m_player1 : m_player2;
			auto card = currentPlayerRef.getCard(move_card_type); //optional cu cartea se extrage cartea din invetaru playerului

			if (card) {
				m_board.appendMove(
					{ posX, posY }, std::move(*card)
				);
				win = (currentPlayer ? m_win_p1 : m_win_p2).won(currentPlayer, posX, posY, m_board);
				if (!win) {
					switchPlayer();
				}
			}
			else {

			}


		}

	}

	bool TrainingMode::_boardIsSet() {
		bool meow = true;

		return meow;
	}

	void TrainingMode::switchPlayer() {
		currentPlayer = !currentPlayer;
	}

	bool TrainingMode::_Won(bool won) {
		return true;
	}

	//--------------------------------------------------Win Manager--------------------------------------------------------

	TrainingMode::WinManager::WinManager(uint16_t size) :
		size{ size },
		diag1{ 0 },
		diag2{ 0 } {

	}

	bool TrainingMode::WinManager::won(uint16_t player, uint16_t row, uint16_t col, const Board& board) {
		rows[{player, row}]++;

		if (rows[{player, row}] == size) {
			return true;
		}

		cols[{player, col}]++;

		if (cols[{player, col}] == size) {
			return true;
		}

		if (board.isFixed()) {
			//board.diagsCount() -> pair <nr elemente diag1, nr element diag2>
		}

		return false;
	}
}

