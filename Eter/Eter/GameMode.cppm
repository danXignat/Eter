export module GameMode;

import <memory>;

import IGameMode;

import Board;

namespace base {
	export class GameMode : public IGameMode {
	public:
		virtual void run() override = 0;

	protected:
		Board m_board;
	};
}