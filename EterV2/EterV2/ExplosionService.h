#pragma once

#include "Board.h"
#include "Player.h"
#include "ExplosionCard.h"

namespace base {
	class ExplosionService {
	public:
		ExplosionService(Board&, Player&, Player&);

		void apply();
		void setting();
		void render_explosion();

	private:
		Board& m_board;
		Player& m_player1, & m_player2;

		Explosion m_card;
	};
}