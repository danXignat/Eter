#pragma once

#include <optional>
#include <functional>

#include "Board.h"
#include "Player.h"
#include "CombatCard.h"
#include "WinManager.h"

#include "typedefs.h"
#include "logger.h"

namespace base {
	class IllusionService {
	public:
		IllusionService(Board&, WinManager&);

		void placeIllusion(const Coord&, CombatCard&& card);
		bool hasIllusion(Player&);
		bool isValidPlaceCard(const Coord&, const CombatCard&);
		static bool hasIllusionWon(CombatCard& illusion, const CombatCard& other);

	private:
		Board& board;
		WinManager& win_manager;

		bool m_p1_has_illusion;
		bool m_p2_has_illusion;

		std::optional<Coord> m_p1_illusion_coord;
		std::optional<Coord> m_p2_illusion_coord;
	};
}