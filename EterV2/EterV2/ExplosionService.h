#pragma once

#include "Board.h"
#include "Player.h"
#include "ExplosionCard.h"

namespace base {
	class ExplosionService {
	public:
		ExplosionService(Board& board, Player& p1, Player& p2);

		void apply();
		void setting();
		void render_explosion();

	private:
		Board& m_board;
		Player& m_p1, & m_p2;

		Explosion m_card;
	};
}