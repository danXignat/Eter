export module TrainingMode;
import <sstream>;
import <optional>;
import <string>;
import <unordered_map>;

import Board;
import Player;
import CombatCard;
import CombatCardType;
import Inventory;

import GameMode;

import IllusionService;
import ExplosionService;

namespace base {
	export class TrainingMode : public GameMode {
	public:
		TrainingMode(bool, bool, const std::string&, const std::string&);
		void run() {

		}
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
			WinManager() = default;
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
		Board m_board;
		Player m_player1, m_player2;

		std::optional<IllusionService> m_illusion_service;
		std::optional<ExplosionService> m_explosion_service;

		WinManager m_win_manager;
		bool m_current_player;
	};
}