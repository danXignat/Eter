module TrainingMode;

import <iostream>;
import <memory>;
import <unordered_map>;

import CombatCard;
import CombatCardType;
import Board;

namespace modes {


	TrainingMode::TrainingMode(std::string player1_name, std::string player2_name) {

		m_player1.setName(player1_name);
		m_player2.setName(player2_name);

	}
	void TrainingMode::gameLoop(bool win) {
		base::Board board(3);
		WinManager m_winManager;
		base::CombatCardType type;
		std::pair<uint16_t, uint16_t>up, down;
		up.first = down.first = 10;
		up.second = down.second = 5;
		while (!win) {
			
			uint16_t posX, posY;
			char card_type;
			std::cout << m_player1.getName();
			std::cin >> posX >> posY >> card_type;
			if (posX > down.first) {
				down.first = posX;
			}
			if (posX < up.first) {
				up.first = posX;
			}
			if (posY > down.second) {
				down.second = posY;
			}
			if (posY < up.second) {
				up.second = posY;
			}
			switch (card_type)
			{
				using enum base::CombatCardType;
			case '1':
				type = ONE;
				break;
			case '2':
				type = TWO;
				break;
			case '3':
				type = THREE;
				break;
			case '4':
				type = FOUR;
				break;
			case 'E':
				type = ETER;
				break;
			default:
				break;
			}

			board.appendMove(
				{ posX, posY }, std::make_unique<base::CombatCard>(type)
			);
			board.renderBoard();
			win = m_winManager.won(posX, posY);
			if (win == 1)
				std::cout << "AM CASTIGAT";
		}
		
	}

	TrainingMode::WinManager::WinManager() {


	}
	bool TrainingMode::WinManager::won(uint16_t row, uint16_t col) {
		rows[row]++;
		std::cout << rows[row] << std::endl;
		if (rows[row] == 3) {
			return 1;
		}
		cols[col]++;
		std::cout << cols[col] << std::endl;
		if (cols[col] == 3) {
			return 1;
		}
		return 0;
	}
	bool TrainingMode::_boardIsSet() {
		bool meow = true;

		return meow;
	}
}
