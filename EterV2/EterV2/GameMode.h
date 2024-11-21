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
#include "ServiceType.h"

namespace base {
	class GameMode : public IGameMode {
	public: 
		GameMode();

	public:
		virtual void run() override = 0;

	protected:
		void _switchPlayer();
		Player& _currPlayer();

	protected:
		struct InputHandler {
			InputHandler() = default;
			void read();

			std::optional<uint16_t> x, y;
			std::optional<CombatCardType> card_type;
			std::optional<ServiceType> service_type;
		};

	protected:
		std::optional<Player> m_player1, m_player2;
		bool m_current_player;
	};
}