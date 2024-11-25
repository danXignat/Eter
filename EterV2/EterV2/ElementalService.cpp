#include"ElementalService.h"

#include<cstdint>
#include<random>
#include<memory>
#include <algorithm>

namespace base {
	ElementalService::ElementalService() {
		std::random_device rd;
		std::mt19937 gen(rd());

		std::array<uint16_t, ElementalService::power_number>choices;
		std::iota(choices.begin(), choices.end(), 0);
		std::shuffle(choices.begin(), choices.end(), gen);

		type_p1 = static_cast<PowerCardType>(choices[0]);
		type_p2 = static_cast<PowerCardType>(choices[1]);

	}
	void ElementalService::selectPowerCards() {
		_setPlayerPowerCard(0);
			_setPlayerPowerCard(1);
	}

		void ElementalService::_setPlayerPowerCard(bool player) {
			std::cout << ((player == 0) ? "Player 1" : "Player 2") << " choose powerCard:" << "\n";
			PowerCardType type=(player == 0) ? type_p1 : type_p2;
			std::cout << "Power card 1: " <<typeToStrings(type_p1)<<": "<< abilityToString(type_p1) << '\n';
			std::cout << "Power card 2: " << typeToStrings(type_p2) << ": " << abilityToString(type_p2) << "\n";

			auto abilities = _getPowerCard(type);
			uint16_t choice;
			std::cin >> choice;

			switch (choice)
			{
			case 1:
				if (player == 0) {
					card_p1 = _factory(abilities);
				}
				else {
					card_p2 = _factory(abilities);
				}
				break;
			case 2:
				if (player == 0) {
					card_p1 = _factory(abilities);
				}
				else {
					card_p2 = _factory(abilities);
				}
				break;
			default:
				std::cout << "Invalid input";
				break;
			}
		}

		PowerCardType ElementalService::_getPowerCard(PowerCardType type) {
			return type;
		}

		std::unique_ptr<PowerCard>ElementalService::_factory(PowerCardType ability) {
			switch (ability){
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