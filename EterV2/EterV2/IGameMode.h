#pragma once

#include "Board.h"
#include "Player.h"

namespace base {
	struct IGameMode {
		virtual void run() = 0;
		virtual const Player& getPlayerRed() const = 0;
		virtual const Player& getPlayerBlue() const = 0;
		virtual const Player& getCurrPlayer() const = 0;
		virtual const Board& getBoard() const = 0;
		virtual ~IGameMode() = default;
	};
}