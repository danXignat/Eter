#pragma once

#include <optional>
#include <functional>

#include "Board.h"
#include "Player.h"
#include "CombatCard.h"

#include "typedefs.h"
#include "logger.h"

namespace base {
	class IllusionService {
	public:
		IllusionService(Board&);

		void add(CombatCard& card);
		void eventSlot(CombatCard& illusion, CombatCard& other);

	private:
		bool m_player1_illusion;
		bool m_player2_illusion;

		Board& board;
	};
}