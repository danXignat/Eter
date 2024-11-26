#pragma once

namespace base {
	struct IGameMode {
		virtual void run() = 0;

	protected:
		virtual void switchPlayer() = 0;
	};
}