#pragma once
#include <optional>

#include "IGameMode.h"
#include "Board.h"
#include "Player.h"
#include "WinManager.h"
#include "IllusionService.h"
#include "ExplosionService.h"
#include "ServiceType.h"

namespace base {
	class BaseGameMode : public IGameMode {
	public:
		BaseGameMode(const std::vector<ServiceType>& services, const std::string& player1_name, const std::string& player2_name);

		virtual void run() = 0;

	protected:

	protected:
		Board m_board;
		WinManager m_win_manager;

		PlayerRef m_curr_player;
		Player m_player_red;
		Player m_player_blue;

		std::optional<IllusionService> m_illusion_service;
		std::optional<ExplosionService> m_explosion_service;
	};
}