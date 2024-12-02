#pragma once

#include "Board.h"
#include "Player.h"
#include "ExplosionCard.h"

namespace base {
	class ExplosionService {
	public:
		ExplosionService(Board& board);

		void apply();
		void setting();
		void render_explosion();

	private:
		Board& m_board;

		Explosion m_card;
	};
}