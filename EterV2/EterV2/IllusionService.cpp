#include "IllusionService.h"

using namespace logger;

namespace base {
	IllusionService::IllusionService(Board& board) 
		: m_p1_has_illusion{ true },
		m_p2_has_illusion{ true },
		board{ board } {
	}

	void IllusionService::add(CombatCard& card) {
		if (card.getColor() == color::ColorType::RED) {
			if (!m_p1_has_illusion) {
				Logger::log(
					Level::WARNING,
					"no more illusions for player RED"
				);
			}
			else {
				card.flip();
				m_p1_has_illusion = false;
			}
		}
		else {
			if (!m_p2_has_illusion) {
				Logger::log(
					Level::WARNING,
					"no more illusions for player BLUE"
				);
			}
			else {
				card.flip();
				m_p2_has_illusion = false;
			}
		}
	}

	void IllusionService::event(CombatCard& illusion, CombatCard& other) {
		if (illusion.getType() > other.getType()) {
			illusion.flip();

			Logger::log(Level::INFO, "Illusion wins and reveal");
		}
		else {
			std::swap(illusion, other);

			Logger::log(Level::INFO, "Attacker defeats illusion");
		}
	}
}