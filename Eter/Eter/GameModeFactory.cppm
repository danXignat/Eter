export module GameModeFactory;

#include <cmath>
import <memory>;
import <iostream>;
import <string>;

import IGameMode;
import TrainingMode;
import MageDuelMode;
import ElementalMode;
import TournamentMode;
import TimedMode;

namespace base {
	export using GameModePtr = std::unique_ptr<IGameMode>;

	export struct GameModeFactory {
		static GameModePtr get(const std::string& id) {
			switch (id.front()) {
				case '0': {
					return std::unique_ptr<TrainingMode>();
				}
				case '1': {
					return std::unique_ptr<MageDuelMode>();
				}
				case '2': {
					return std::unique_ptr<ElementalMode>();
				}
				case '3': {
					return std::unique_ptr<TournamentMode>();
				}
				case '4': {
					return std::unique_ptr<TimedMode>();
				}
				default: {
					return nullptr;
				}
			}
		}
	};

	
}