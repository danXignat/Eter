#pragma once
#include <string>
#include <optional>

#include "BaseGameMode.h"
#include "WinManager.h"
#include "ExplosionService.h"
#include "ElementalService.h"
#include "ServiceType.h"
#include "GameModeType.h"
#include "ArenaService.h"

namespace base {
	class TournamentMode : public BaseGameMode {
	public:
		TournamentMode(const std::vector<ServiceType>& services, const std::pair<std::string, std::string>& player_names,
						std::unique_ptr<BaseGameMode>base_mode);
		void run() override;
		void render();

	private:
		ArenaService m_arena_service;
		std::unique_ptr<BaseGameMode> m_base_mode;
	};

}