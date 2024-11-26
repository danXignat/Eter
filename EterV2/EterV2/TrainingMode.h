#pragma once
#include <string>
#include <optional>

#include "Player.h"
#include "typedefs.h"
#include "IGameMode.h"
#include "WinManager.h"
#include "IllusionService.h"
#include "ExplosionService.h"
#include "ServiceType.h"

namespace base {
	class TrainingMode : public IGameMode {
	public:
		TrainingMode(std::vector<ServiceType>, const std::string&, const std::string&);
		void run() override;

	protected:
		void switchPlayer() override;
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