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
		TournamentMode(const std::vector<ServiceType>& services, const std::pair<std::string, std::string>& player_names);

		void run() override;
		void render();
		bool processGameInput(const InputHandler& input);
		void displayModeSelection();
		void handleGameEnd();
		void updateScores();

	private:
		static const uint16_t MAX_GAMES = 5;
		uint16_t m_games_played;
		uint16_t m_red_wins;
		uint16_t m_blue_wins;
		bool m_tournament_ended;
		std::pair<std::string, std::string> m_player_names;

		ArenaService m_arena_service;
		std::unique_ptr<BaseGameMode> m_base_mode;
		//std::optional <MageMode>m_mage_mode;
		//std::optional<ElementalMode>m_elemental_mode;
	};

}