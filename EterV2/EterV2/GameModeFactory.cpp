#include "GameModeFactory.h"

namespace base {
	GameModePtr GameModeFactory::get(const std::string& id, const std::string& name1, const std::string& name2) {
		switch (id.front()) {
		case '1': {
			return std::make_unique<TrainingMode>(id[1] - 48, id[2] - 48, name1, name2);
		}
		case '2': {
			//return std::unique_ptr<MageDuelMode>();
			return nullptr;
		}
		case '3': {
			//return std::unique_ptr<ElementalMode>();
			return nullptr;
		}
		case '4': {
			//return std::unique_ptr<TournamentMode>();
			return nullptr;
		}
		case '5': {
			//return std::unique_ptr<TimedMode>();
			return nullptr;
		}
		default: {
			return nullptr;
		}
		}
	}
}
