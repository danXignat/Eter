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
	class BaseGameMode : public IGameMode {
	public:
		BaseGameMode(GameSizeType, const std::pair<std::string, std::string>&, const std::vector<ServiceType>&);
		virtual ~BaseGameMode() = default;

		virtual void run() = 0;

	public:
		bool placeCombatCard(const InputHandler&);

		virtual bool placeIllusion(const InputHandler&);

		virtual bool isExplosionAvailable();
		virtual void setExplosion();
		virtual void detonateExplosion();

		virtual bool useMage();

		virtual bool usePower();

		void switchPlayer();

	protected:
		Player m_player_red;
		Player m_player_blue;
		PlayerRef m_curr_player;

		Board m_board;
		WinManager m_win_manager;

		std::optional<IllusionService> m_illusion_service;
		std::optional<ExplosionService> m_explosion_service;
	};
}