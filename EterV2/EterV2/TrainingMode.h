#pragma once
#include <string>
#include <optional>

#include "IGameMode.h"
#include "IllusionService.h"
#include "ExplosionService.h"
#include "WinManager.h"
#include "ServiceType.h"
#include "Player.h"
#include "typedefs.h"

namespace base {
	class TrainingMode : public IGameMode {
	public:
		TrainingMode(const std::vector<ServiceType>&, const std::string&, const std::string&);
		void run();
		void switchPlayer();
		void render();

	private:
		Board m_board;
		WinManager m_win_manager;

		std::optional<IllusionService> m_illusion_service;
		std::optional<ExplosionService> m_explosion_service;

		PlayerRef curr_player;
		Player m_player_red;
		Player m_player_blue;
	};
}