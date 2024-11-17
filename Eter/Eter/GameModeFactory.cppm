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
		static GameModePtr get(const std::string& id, const std::string& name1, const std::string& name2) {
			switch (id.front()) {
				case '1': {
					return std::make_unique<TrainingMode>(id[1] - 48, id[2] - 48, name1, name2);
				}
				case '2': {
					return std::unique_ptr<MageDuelMode>();
				}
				case '3': {
					return std::unique_ptr<ElementalMode>();
				}
				case '4': {
					return std::unique_ptr<TournamentMode>();
				}
				case '5': {
					return std::unique_ptr<TimedMode>();
				}
				default: {
					return nullptr;
				}
			}
		}
	};

	
}