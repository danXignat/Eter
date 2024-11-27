#pragma once
#include <string>
#include <optional>

#include "IGameMode.h"
#include "WinManager.h"
#include "ExplosionService.h"
#include "ElementalService.h"
#include "ServiceType.h"

namespace base {
	class ElementalMode : public IGameMode {
	public:
		ElementalMode(const std::vector<ServiceType>&, const std::string&, const std::string&);
		void run();
		void render();
		void switchPlayer();

	private:
		Board m_board;
		WinManager m_win_manager;

		std::optional<IllusionService> m_illusion_service;
		std::optional<ExplosionService> m_explosion_service;
		ElementalService m_elemental_service;

		PlayerRef curr_player;
		Player m_player_red;
		Player m_player_blue;
	};
}