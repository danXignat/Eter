#pragma once
#include <string>
#include <optional>

#include "IGameMode.h"
#include "WinManager.h"
#include "IllusionService.h"
#include "ExplosionService.h"
#include "MageService.h"
#include "ServiceType.h"

namespace base {
	class MageMode : public IGameMode {
	public:
		MageMode(const std::vector<ServiceType>&, const std::string&, const std::string&);
		void run();
		void render();
		void switchPlayer();

	private:
		Board m_board;
		WinManager m_win_manager;

		std::optional<IllusionService> m_illusion_service;
		std::optional<ExplosionService> m_explosion_service;
		MageService m_mage_service;

		PlayerRef curr_player;
		Player m_player_red;
		Player m_player_blue;
	};
}