#pragma once

namespace base {
	struct IGameMode {
		virtual void run() = 0;

		virtual ~IGameMode() = default;

	protected:
		virtual void switchPlayer() = 0;
	};
}