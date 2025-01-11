#pragma once
#include <string>
#include <optional>

#include "BaseGameMode.h"
#include "WinManager.h"
#include "ExplosionService.h"
#include "ElementalService.h"
#include "ServiceType.h"
#include "GameModeType.h"
#

namespace base {
	class TournamentMode : public BaseGameMode {
	public:
		TournamentMode(const std::vector<ServiceType>&, const std::pair<std::string, std::string>&);
		void run() override;
		void render();

	private:
		ArenaService m_arena_service;
	};

}