export module TrainingMode;
import <sstream>;
import <optional>;
import <string>;
import <unordered_map>;

import Board;
import Player;
import CombatCard;
import CombatCardType;

namespace base {
	export class TrainingMode {
	public:
		TrainingMode(std::string, std::string);

	    void gameLoop();

	protected:
		void _switchPlayer();
		Player& _curentPlayer();
		CombatCardType _fromCharToType(char);

	protected:
		 struct WinManager {
			uint16_t size;
			std::optional<int16_t> diag1, diag2;
			std::unordered_map<uint16_t, int16_t> rows, cols;

			WinManager(uint16_t);
			 bool won(const Coord&, const Player&, const Board&);
		};

		struct Input {
			uint16_t x, y;
			char card_type;
			bool illusion;

			Input();
		};

	protected:
		WinManager m_win_manager;
		Player m_player1, m_player2;
		Board m_board;
		bool m_current_player;
	};
}