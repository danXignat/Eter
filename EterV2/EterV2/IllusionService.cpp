#include "IllusionService.h"

using namespace logger;

namespace base {
	IllusionService::IllusionService(Board& board, WinManager& win_manager) 
		: m_p1_has_illusion{ true },
		m_p2_has_illusion{ true },
		board{ board },
		win_manager{ win_manager } {
	}

	bool IllusionService::hasIllusion(Player& player) {
		if (player.getColor() == color::ColorType::RED) {
			return m_p1_has_illusion;
		}
		else {
			return m_p2_has_illusion;
		}
	}

	void IllusionService::placeIllusion(const Coord& coord, CombatCard&& card) {
		if (card.getColor() == color::ColorType::RED) {
			if (!m_p1_has_illusion) {
				Logger::log(
					Level::WARNING,
					"no more illusions for player RED"
				);
			}
			else {
				board.appendMove(coord, std::move(card));
				win_manager.addCard(coord);
				m_p1_illusion_coord.emplace(coord);
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
				board.appendMove(coord, std::move(card));
				win_manager.addCard(coord);
				m_p2_illusion_coord.emplace(coord);
				m_p2_has_illusion = false;
			}
		}
	}

	bool IllusionService::isValidPlaceCard(const Coord& coord, const CombatCard& card) {
		if (card.isIllusion() && board.hasStack(coord) == false) {
			return true;
		}
		
		return false;
	}

	bool IllusionService::hasIllusionWon(CombatCard& illusion, const CombatCard& other) {
		illusion.flip();

		if (other < illusion) {
			return true;
			Logger::log(Level::INFO, "Illusion wins and reveal");
		}
		else {
			return false;
			Logger::log(Level::INFO, "Attacker defeats illusion");
		}
	}
}