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
		m_curr_player{ m_player_red },
		m_game_size_type{game_size},
		m_current_round{0},
		m_round_count{ (game_size == GameSizeType::SMALL) ? uint16_t(3) : uint16_t(5) } {

		for (ServiceType service : services) {
			switch (service) {
				using enum ServiceType;
			case ILLUSION:
				m_illusion_service.emplace(m_board, m_player_red, m_player_blue);
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

	GameSizeType BaseGameMode::getGameSizeType() const {
		return m_game_size_type;
	}

	std::optional<color::ColorType> BaseGameMode::getWinningColor() const {
		if (auto win_data = m_board.getWinData(); win_data.has_value()) {
			return win_data->second;
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
		/// pentru chestia asta ca sa fie mega clean cred ca mergea un observer design pattern
		bool is_illusion{ m_board.getTopCard(input.coord).has_value() &&  m_board.getTopCard(input.coord)->get().isIllusion() };
		if (is_illusion && m_illusion_service.has_value()) {
			m_illusion_service->handleIllusionAttack(input);
			return true;
		}

		if (m_board.isValidPlaceCard(input.coord, m_curr_player.get().viewCard(input.card_type))) {
			if (Config::getInstance().getFetchByID()) {
				CombatCard card = m_curr_player.get().getCardByID(input.ID);
				m_board.appendMove(input.coord, std::move(card));
			}
			else {
				CombatCard card = m_curr_player.get().getCard(input.card_type);
				m_board.appendMove(input.coord, std::move(card));
			}

			return true;
		}

		return false;
	}

	std::optional <ExplosionService>& BaseGameMode::getExplosionService() {
		return m_explosion_service;
	}

	std::optional <IllusionService>& BaseGameMode::getIllusionService() {
		return m_illusion_service;
	}

	void BaseGameMode::removeExplosion() {
		m_explosion_service.reset();
	}

	void BaseGameMode::addExplosion() {
		m_explosion_service.reset();
		m_explosion_service.emplace(m_board, m_player_red, m_player_blue);
	}

	std::unordered_set<uint16_t> BaseGameMode::getAvailableIds(InputHandler& input) {
		std::unordered_set<uint16_t> ids;

		for (const Coord& coord : m_board | std::views::keys) {
			input.coord = coord;

			if (m_board.isValidPlaceCard(input)) {
				ids.insert(m_board[coord].back().getID());
			}
		}

		return ids;
	}

	std::unordered_set<uint16_t> BaseGameMode::getRestrictedIds(InputHandler& input) {
		std::unordered_set<uint16_t> ids;

		for (const Coord& coord : m_board | std::views::keys) {
			input.coord = coord;

			if (!m_board.isValidPlaceCard(input)) {
				ids.insert(m_board[coord].back().getID());
			}
		}

		return ids;
	}

	void BaseGameMode::tryShiftBoard() {

	}

	void BaseGameMode::nextRound() {
		auto win_data{ m_board.getWinData() };
		if (!win_data.has_value()) {
			throw std::runtime_error("no one has won");
		}

		if (win_data->second == color::ColorType::RED) {
			m_score.red++;
		}
		else if (win_data->second == color::ColorType::BLUE) {
			m_score.blue++;
		}

		if (m_current_round < m_round_count) {
			m_board.reset();
			m_player_red.reset();
			m_player_blue.reset();
		}

		m_current_round++;
	}

	uint16_t BaseGameMode::getScore(color::ColorType color) const {
		if (color == color::ColorType::RED) {
			return m_score.red;
		}
		else if (color == color::ColorType::BLUE) {
			return m_score.blue;
		}
	}

	uint16_t BaseGameMode::getCurrRound() {
		return m_current_round;
	}
}