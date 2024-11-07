export module TrainingMode;
import <string>;
import <set>;
import <unordered_map>;

import Board;
import Player;
import CombatCard;
export namespace modes {
	class TrainingMode {
	public:
		TrainingMode(std::string, std::string);

		void gameLoop(bool);
		void switchPlayer();
	
	private:
		bool _boardIsSet();
		bool _notWon(bool);
	private:
		struct WinManager {
			WinManager();
			std::unordered_map<uint16_t, uint16_t> rows, cols;
			bool won(uint16_t, uint16_t);
		};

	private:
		player::Player m_player1;
		player::Player m_player2;
	};
}