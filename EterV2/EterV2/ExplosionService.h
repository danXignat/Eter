#pragma once

#include "Board.h"
#include "Player.h"
#include "ExplosionCard.h"

namespace base {
	class ExplosionService {
	public:
		Explosion card;

	public:
		static constexpr const uint16_t VALID_COMPLETED_LINES = 2;

		ExplosionService(Board&, Player&, Player&);

		void apply();
		void setting();
		void renderExplosion();
		bool checkAvailability() const;
		std::unordered_map<Coord, Effect, utils::CoordFunctor> getEffectCoords() const;

	private:
		Board& m_board;
		Player& m_player1, & m_player2;

	};
}