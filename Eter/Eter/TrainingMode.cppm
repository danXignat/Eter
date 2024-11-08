#include <optional>
export module TrainingMode;
import <string>;
import <set>;
import <unordered_map>;

import Board;
import Player;
import CombatCard;

namespace base {
	class TrainingMode {
	public:
		TrainingMode() = default;
		TrainingMode(std::string, std::string);

		void gameLoop(bool);
		void switchPlayer();
	
	private:
		bool _boardIsSet();
		bool _Won(bool);
	private:
		struct WinManager {
			uint16_t size;
			uint16_t diag1, diag2;
			std::unordered_map<uint16_t, uint16_t> rows, cols;

			WinManager() = default;
			WinManager(uint16_t);
			bool won(uint16_t, uint16_t, const Board&);
		};

	private:
		WinManager m_win_p1, m_win_p2;
		Player m_player1, m_player2;
		Board m_board;
	};
}