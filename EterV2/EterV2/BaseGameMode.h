#pragma once
#include <optional>

#include "IGameMode.h"
#include "Board.h"
#include "Player.h"
#include "WinManager.h"
#include "IllusionService.h"
#include "ExplosionService.h"
#include "GameModeType.h"
#include "InputHandler.h"

namespace base {
	struct Score {
		uint16_t red = 0, blue = 0;
	};

	class BaseGameMode : public IGameMode {
	public:
		BaseGameMode(GameSizeType, const std::pair<std::string, std::string>&, const std::vector<ServiceType>&);
		virtual ~BaseGameMode() = default;

		virtual void run() = 0;

	public:
		const Player& getPlayerRed() const override;
		const Player& getPlayerBlue() const override;
		const Player& getCurrPlayer() const override;
		const Board&  getBoard() const override;
		GameSizeType getGameSizeType() const;
		uint16_t getScore(color::ColorType) const;
		uint16_t getCurrRound();

		std::unordered_set<uint16_t> getAvailableIds(InputHandler&);
		std::unordered_set<uint16_t> getRestrictedIds(InputHandler&);

		std::optional <ExplosionService> &getExplosionService();
		std::optional <IllusionService> &getIllusionService();
		std::optional<color::ColorType> getWinningColor() const;

		virtual void nextRound();
		void switchPlayer();
		void tryShiftBoard();
		void removeExplosion();
		void addExplosion();
	
		bool placeCombatCard(const InputHandler&);
		bool placeIllusion(const InputHandler&);

	protected:
		Player m_player_red;
		Player m_player_blue;
		PlayerRef m_curr_player;
		Board m_board;

		std::optional<IllusionService> m_illusion_service;
		std::optional<ExplosionService> m_explosion_service;

		GameSizeType m_game_size_type;
		uint16_t m_round_count;
		uint16_t m_current_round;
		Score m_score;
	};
}