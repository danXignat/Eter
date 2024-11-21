#include "IllusionService.h"

using namespace logger;

namespace base {
	IllusionService::IllusionService(Board& board) 
		: m_player1_illusion{ true },
		m_player2_illusion{ true },
		board{ board } {
	}

	void IllusionService::add(CombatCard& card) {
		if (card.getColor() == color::ColorType::RED) {
			if (!m_player1_illusion) {
				Logger::log(
					Level::WARNING,
					"no more illusions for player RED"
				);
			}
			else {
				card.flip();
				m_player1_illusion = false;
			}
		}
		else {
			if (!m_player2_illusion) {
				Logger::log(
					Level::WARNING,
					"no more illusions for player BLUE"
				);
			}
			else {
				card.flip();
				m_player2_illusion = false;
			}
		}
	}

	void IllusionService::eventSlot(CombatCard& illusion, CombatCard& other) {
		if (illusion.getType() > other.getType()) {
			illusion.flip();
		}
		else {
			std::swap(illusion, other);
		}
	}
}