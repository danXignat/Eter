export module TrainingMode;
import <string>;
import <set>;
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
		bool _notWon(bool);
	private:
		struct WinManager {
			WinManager();

		};

	private:
		player::Player m_player1;
		player::Player m_player2;
	};
}