#pragma once
#include <memory>
#include <unordered_map>
#include <string>
#include <iostream>
#include <sstream>
#include <optional>

#include "IGameMode.h"

#include "Board.h"
#include "Player.h"
#include "CombatCard.h"

#include "utils.h"
#include "typedefs.h"
#include "colors.h"

namespace base {
	class GameMode : public IGameMode {
	public: 
		GameMode();

	public:
		virtual void run() override = 0;

	protected:
		void _switchPlayer();
		Player& _curentPlayer();
		CombatCardType _fromCharToType(char);

	protected:
		struct WinManager {
			Board& board;
			uint16_t size;
			std::optional<int16_t> diag1, diag2;
			std::unordered_map<uint16_t, int16_t> rows, cols;
			WinManager(uint16_t, Board&);
			bool won(const Coord&, const Player&);
		};

		struct Input {
			uint16_t x, y;
			char card_type;
			bool illusion;

			Input() = default;
			void read();
		};

	protected:
		std::optional<Player> m_player1, m_player2;
		bool m_current_player;
	};
}