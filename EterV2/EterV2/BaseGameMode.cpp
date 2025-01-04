#include "BaseGameMode.h"

#include "logger.h"

namespace base {
	BaseGameMode::BaseGameMode(
		GameSizeType game_size,
		const std::pair<std::string, std::string>& player_names,
		const std::vector<ServiceType>& services
	) :
		m_win_manager{ m_board },
		m_player_red{ player_names.first, color::ColorType::RED, game_size },
		m_player_blue{ player_names.second, color::ColorType::BLUE, game_size },
		m_board{ game_size, m_player_red, m_player_blue },
		m_curr_player{ m_player_red } {

		for (ServiceType service : services) {
			switch (service) {
				using enum ServiceType;
			case ILLUSION:
				m_illusion_service.emplace(m_board, m_win_manager);
				break;
			case EXPLOSION:
				m_explosion_service.emplace(m_board, m_player_red, m_player_blue);
				break;
			default:
				break;
			}
		}
	}

	void BaseGameMode::switchPlayer() {
		if (m_curr_player.get().getColor() == color::ColorType::RED) {
			m_curr_player = m_player_blue;
		}
		else {
			m_curr_player = m_player_red;
		}
	}

	bool BaseGameMode::placeCombatCard(const InputHandler& input) {
		CombatCardType card_type = input.card_type;
		Coord coord = input.coord;
		const CombatCard& card_view = m_curr_player.get().viewCard(card_type);

		auto board_card = m_board.getTopCard(coord);
		bool is_illusion = board_card && board_card->get().isIllusion();
		if (is_illusion && IllusionService::hasIllusionWon(board_card.value(), card_view)) {
			logger::Logger::log(logger::Level::INFO, "illusion has won");
			return true;
		}

		if (m_board.isValidPlaceCard(coord, card_view)) {
			CombatCard card = m_curr_player.get().getCard(card_type);

			m_board.appendMove(coord, std::move(card));

			m_win_manager.addCard(coord);

			return true;
		}

		return false;
	}

	bool BaseGameMode::placeIllusion(const InputHandler& input) {
		if (m_illusion_service) {
			Coord coord = input.coord;
			CombatCardType card_type = input.card_type;

			CombatCard card = m_curr_player.get().getCard(card_type);
			card.flip();

			bool has_illusion = m_illusion_service->hasIllusion(m_curr_player.get());
			bool valid_move = m_illusion_service->isValidPlaceCard(coord, card);

			if (has_illusion && valid_move) {
				m_illusion_service->placeIllusion(coord, std::move(card));

				return true;
			}
			else {
				card.flip();

				m_curr_player.get().addCard(std::move(card));
			}
		}
		else {
			return false;
		}
	}

	bool BaseGameMode::isExplosionAvailable() {
		if (m_explosion_service.has_value()) {
			return m_explosion_service->checkAvailability();
		}
		
		return false;
	}

	void BaseGameMode::setExplosion() {
		m_explosion_service->setting();
	}

	void BaseGameMode::detonateExplosion() {
		m_explosion_service->apply();
	}

	bool BaseGameMode::useMage() {
		return false;
	}

	bool BaseGameMode::usePower() {
		return false;
	}
}