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
		static void event(CombatCard& illusion, CombatCard& other);

	private:
		bool m_p1_has_illusion;
		bool m_p2_has_illusion;

		Board& board;
	};
}