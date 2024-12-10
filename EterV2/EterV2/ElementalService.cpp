#include"ElementalService.h"

#include <cstdint>
#include <random>
#include <memory>
#include <algorithm>

#include "logger.h"
#include "utils.h"

using namespace logger;

namespace base {
	ElementalService::ElementalService(Board& board) : m_board{ board } {
		std::random_device rd;
		std::mt19937 gen(rd());

		std::array<uint16_t, ElementalService::power_number> choices;
		std::iota(choices.begin(), choices.end(), 0);
		std::shuffle(choices.begin(), choices.end(), gen);

		/*type_p1 = static_cast<PowerCardType>(choices[0]);
		type_p2 = static_cast<PowerCardType>(choices[1]);*/

		type_card1 = PowerCardType::Mirrage;
		type_card2 = PowerCardType::Mirrage;

		card1 = _factory(type_card1);
		card2 = _factory(type_card2);
	}

	void ElementalService::apply(char choice, Player& player) {
		switch (choice) {
		case '1': {
			card1->apply(m_board, player);
			card1.reset();
			break;
		}

		case '2': {
			card2->apply(m_board, player);
			card2.reset();
			break;
		}

		default: {
			Logger::log(Level::WARNING, "choice invalid");
			break;
		}

		}
		
	}

	void ElementalService::renderCards() const {
		if (card1) {
			utils::printAtCoordinate(
				std::format("Power 1 << {} >> {}", typeToStrings(type_card1), abilityToString(type_card1)),
				1, 25
			);
		}

		if (card2) {
			utils::printAtCoordinate(
				std::format("Power 2 << {} >> {}", typeToStrings(type_card2), abilityToString(type_card2)),
				1, 26
			);
		}
	}

	std::unique_ptr<PowerCard>ElementalService::_factory(PowerCardType ability) {
		switch (ability) {
		case PowerCardType::ControllerExplosion:
			return std::make_unique<ControllerExplosion>();

		case PowerCardType::Destruction:
			return std::make_unique<Destruction>();

		case PowerCardType::Flame:
			return std::make_unique<Flame>();

		case PowerCardType::Fire:
			return std::make_unique<Fire>();

		case PowerCardType::Ash:
			return std::make_unique<Ash>();

		case PowerCardType::Spark:
			return std::make_unique<Spark>();

		case PowerCardType::Squall:
			return std::make_unique<Squall>();

		case PowerCardType::Gale:
			return std::make_unique<Gale>();

		case PowerCardType::Hurricane:
			return std::make_unique<Hurricane>();

		case PowerCardType::Gust:
			return std::make_unique<Gust>();

		case PowerCardType::Mirrage:
			return std::make_unique<Mirrage>();

		case PowerCardType::Storm:
			return std::make_unique<Storm>();

		case PowerCardType::Tide:
			return std::make_unique<Tide>();

		case PowerCardType::Mist:
			return std::make_unique<Mist>();

		case PowerCardType::Wave:
			return std::make_unique<Wave>();

		case PowerCardType::Whirlpool:
			return std::make_unique<Whirlpool>();

		case PowerCardType::Blizzard:
			return std::make_unique<Blizzard>();

		case PowerCardType::Waterfall:
			return std::make_unique<Waterfall>();

		case PowerCardType::Support:
			return std::make_unique<Support>();

		case PowerCardType::Earthquake:
			return std::make_unique<Earthquake>();

		case PowerCardType::Crumble:
			return std::make_unique<Crumble>();

		case PowerCardType::Border:
			return std::make_unique<Border>();

		case PowerCardType::Avalanche:
			return std::make_unique<Avalanche>();

		case PowerCardType::Rock:
			return std::make_unique<Rock>();
		default:
			return nullptr;
		}
	}
}