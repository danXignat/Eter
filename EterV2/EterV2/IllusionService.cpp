#include "IllusionService.h"

using namespace logger;

namespace base {
	IllusionService::IllusionService(Board& board, Player& player_red, Player& player_blue)
		: m_p1_has_illusion{ true },
		m_p2_has_illusion{ true },
		m_board{ board },
		m_player_red{player_red},
		m_player_blue{player_blue} {
	}

	bool IllusionService::hasIllusion(Player& player) {
		if (player.getColor() == color::ColorType::RED) {
			return m_p1_has_illusion;
		}
		else {
			return m_p2_has_illusion;
		}
	}

	bool IllusionService::hasIllusion(color::ColorType color) {
		if (color == color::ColorType::RED) {
			return m_p1_has_illusion;
		}
		else {
			return m_p2_has_illusion;
		}
	}

	bool IllusionService::placeIllusion(const InputHandler& input) {
		if (input.event_type != EventType::PlaceIllusion) {
			throw std::runtime_error("not an illusion");
		}

		if (m_board.availableSpaces().contains(input.coord) == false) {
			return false;
		}

		Player& curr_player{ m_player_red.hasCard(input.ID) ? m_player_red : m_player_blue };

		CombatCard card = curr_player.getCardByID(input.ID);

		card.flip();

		m_board.appendMove(input.coord, std::move(card));

		if (input.color == color::ColorType::RED) {
			m_p1_illusion_coord.emplace(input.coord);
			m_p1_has_illusion = false;
		}
		else {
			m_p2_illusion_coord.emplace(input.coord);
			m_p2_has_illusion = false;
		}

		return true;
	}

	void IllusionService::handleIllusionAttack(const InputHandler& input) {
		auto& illusion{ m_board.getTopCard(input.coord)->get() };
		if (illusion.isIllusion() == false) {
			throw std::runtime_error("not an illusion");
		}

		Player& curr_player{ (m_player_red.hasCard(input.ID)) ? m_player_red : m_player_blue };
		CombatCard card = curr_player.getCardByID(input.ID);

		if (illusion < card) {
			m_board.appendMove(input.coord, std::move(card));
		}
		else {
			curr_player.addUsedCard(std::move(card));
		}

		if (input.color == color::ColorType::RED) {
			m_p1_illusion_coord.reset();
		}
		else {
			m_p2_illusion_coord.reset();
		}

		m_board[input.coord].front().flip();
	}

	void IllusionService::getNewIllusion(const CombatCard& card) {
		if (card.getColor() == color::ColorType::RED) {
			m_p1_has_illusion = true;
		}
		else {
			m_p2_has_illusion = true;
		}
	}

	std::optional<Coord> IllusionService::getIllusionCoord(color::ColorType color) {
		if (color == color::ColorType::RED) {
			return m_p1_illusion_coord;
		}
		else if (color == color::ColorType::BLUE){
			return m_p2_illusion_coord;
		}
	}
}