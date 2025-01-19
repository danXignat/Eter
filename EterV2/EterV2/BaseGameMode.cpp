#include "BaseGameMode.h"

#include "logger.h"
#include "Config.h"

namespace base {
	BaseGameMode::BaseGameMode(
		GameSizeType game_size,
		const std::pair<std::string, std::string>& player_names,
		const std::vector<ServiceType>& services
	) :
		m_player_red{ player_names.first, color::ColorType::RED, game_size },
		m_player_blue{ player_names.second, color::ColorType::BLUE, game_size },
		m_board{ game_size, m_player_red, m_player_blue },
		m_curr_player{ m_player_red } {

		for (ServiceType service : services) {
			switch (service) {
				using enum ServiceType;
			case ILLUSION:
				//m_illusion_service.emplace(m_board, m_win_manager);
				break;
			case EXPLOSION:
				m_explosion_service.emplace(m_board, m_player_red, m_player_blue);
				break;
			default:
				break;
			}
		}
	}

	const Player& BaseGameMode::getPlayerRed() const {
		return m_player_red;
	}

	const Player& BaseGameMode::getPlayerBlue() const {
		return m_player_blue;
	}

	const Player& BaseGameMode::getCurrPlayer() const {
		return m_curr_player.get();
	}

	const Board& BaseGameMode::getBoard() const {
		return m_board;
	}

	std::optional<color::ColorType> BaseGameMode::getWinningColor() const {
		if (auto win_coord = m_board.getWinCoord(); win_coord.has_value()) {
			return m_board[*win_coord].back().getColor();
		}

		return std::nullopt;
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
			if (Config::getInstance().getFetchByID()) {
				CombatCard card = m_curr_player.get().getCardByID(input.ID);
				m_board.appendMove(coord, std::move(card));
			}
			else {
				CombatCard card = m_curr_player.get().getCard(card_type);
				m_board.appendMove(coord, std::move(card));
			}

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

	std::optional <ExplosionService>& BaseGameMode::getExplosionService() {
		return m_explosion_service;
	}

	void BaseGameMode::removeExplosion() {
		m_explosion_service.reset();
	}
}