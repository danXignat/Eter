#pragma once
#include <string>
#include <optional>

#include "BaseGameMode.h"
#include "WinManager.h"
#include "ExplosionService.h"
#include "ElementalService.h"
#include "ElementalMode.h"
#include "ServiceType.h"
#include "GameModeType.h"
#include "ArenaService.h"

namespace base {
	class TournamentMode : public BaseGameMode {
	public:
		TournamentMode(const std::vector<ServiceType>& services, const std::pair<std::string, std::string>& player_names);

		void run() override;
		void render();
		

	private:
		

	private:
		ArenaService m_arena_service;
		
		std::optional<MageService> m_mage_service;
		std::optional<ElementalService> m_elemental_service;
	};

}