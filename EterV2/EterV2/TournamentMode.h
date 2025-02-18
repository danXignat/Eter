#pragma once
#include <string>
#include <optional>

#include "BaseGameMode.h"
#include "GameModeType.h"
#include "ServiceType.h"
#include "MageService.h"
#include "ElementalService.h"
#include "ArenaService.h"

namespace base {
	class TournamentMode : public BaseGameMode {
	public:
		TournamentMode(const std::vector<ServiceType>& services, const std::pair<std::string, std::string>& player_names);

		void run() override;
		void render();
		void nextRound() override;

		std::optional<MageService>& getMageService();
		std::optional<ElementalService>& getElementalService();
		ArenaService& getArenaService();

	private:
		

	private:
		std::optional<MageService> m_mage_service;
		std::optional<ElementalService> m_elemental_service;
		ArenaService m_arena_service;
	};

}