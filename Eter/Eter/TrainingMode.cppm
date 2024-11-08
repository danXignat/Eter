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

	private:
		bool _boardIsSet();
		bool _Won(bool);
		void switchPlayer();
	private:
		struct WinManager {
			uint16_t size;
			uint16_t diag1, diag2;

			//cand ne am vazut la aula am vazut ca Dan facuse ceva de genu pentru funtia
			//de hash si am zis ca de dragut sa am si eu aici una
			struct PairHash {
				template <class T1, class T2>
				std::size_t operator()(const std::pair<T1, T2>& pair) const {
					return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
				}
			};

			std::unordered_map<std::pair<uint16_t, uint16_t>, uint16_t, PairHash> rows, cols;
			//am pus pair<uint16_t,uint16_t> ca sa tin minte si ce player am
			//prima valoare e pentru player, a 2a este pozitia
			WinManager() = default;
			WinManager(uint16_t);
			bool won(uint16_t, uint16_t, uint16_t, const Board&);

		};

	private:
		WinManager m_win_p1, m_win_p2;
		Player m_player1, m_player2;
		bool currentPlayer = true;//true = player 1, false = player 2
		Board m_board;
	};
}