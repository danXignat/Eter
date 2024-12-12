#pragma once
#include <optional>

#include "IGameMode.h"
#include "Board.h"
#include "Player.h"
#include "WinManager.h"
#include "IllusionService.h"
#include "ExplosionService.h"
#include "GameModeType.h"
#include "InputHandler.h"

namespace base {
	class BaseGameMode : public IGameMode {
	public:
		BaseGameMode(GameSizeType, const std::pair<std::string, std::string>&);
		virtual ~BaseGameMode() = default;

		virtual void run() = 0;

	protected:
		bool _handleEvent(const InputHandler&);
		bool _handleBoardEvent(const InputHandler&);
		virtual bool _handleSpecialEvent(const InputHandler&) = 0;

		void _switchPlayer();

	protected:
		Player m_player_red;
		Player m_player_blue;
		PlayerRef m_curr_player;

		Board m_board;
		WinManager m_win_manager;
	};
}