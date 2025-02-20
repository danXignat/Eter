#pragma once

#include <optional>
#include <functional>

#include "Board.h"
#include "Player.h"
#include "CombatCard.h"
#include "InputHandler.h"

#include "logger.h"

namespace base {
	class IllusionService {
	public:
		IllusionService(Board&, Player&, Player&);

		bool placeIllusion(const InputHandler& input);
		bool hasIllusion(Player&);
		bool hasIllusion(color::ColorType);
		void getNewIllusion(const CombatCard& card);
		void handleIllusionAttack(const InputHandler& input);
		std::optional<Coord> getIllusionCoord(color::ColorType color);

	private:
		Board&  m_board;
		Player& m_player_red;
		Player& m_player_blue;

		bool m_p1_has_illusion;
		bool m_p2_has_illusion;

		std::optional<Coord> m_p1_illusion_coord;
		std::optional<Coord> m_p2_illusion_coord;
	};
}