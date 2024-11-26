#include "MageService.h"

#include<memory>
#include <random>

namespace base {
	MageService::MageService(Board& board) : m_board{ board } {
		std::random_device rd;
		std::mt19937 gen(rd());

		std::array<uint16_t, MageService::mage_number> choices;
		std::iota(choices.begin(), choices.end(), 0);
		std::shuffle(choices.begin(), choices.end(), gen);

		uint16_t choice_p1 = choices[0];
		uint16_t choice_p2 = choices[1];

		/*type_p1 = _getTypeChoice(choice_p1);
		type_p2 = _getTypeChoice(choice_p2);*/

		type_p1 = MageType::Fire;
		type_p2 = MageType::Fire;
	}
	
	void MageService::selectMages() {
		_setPlayerMage(0);
		_setPlayerMage(1);
	}

	void MageService::apply(Player& player) {
		if (player.getColor() == color::ColorType::RED) {
			card_p1->apply(m_board, player);
		}
		else {
			card_p2->apply(m_board, player);
		}
	}

	void MageService::_setPlayerMage(bool player) {
		std::cout << ((player == 0) ? "Player 1" : "Player 2") << " choose mage:\n";
		MageType type = (player == 0) ? type_p1 : type_p2;
		std::cout << "Mage: " << typeToStrings(type) << "\n";

		auto abilities = _getAbilityForType(type);
		std::cout << "Choice 1: " << abilityToString(abilities.first) << '\n';
		std::cout << "Choice 2: " << abilityToString(abilities.second) << '\n';

		uint16_t choice;
		std::cin >> choice;

		switch (choice)
		{
		case 1:
			if (player == 0) {
				card_p1 = _factory(abilities.first);
			}
			else {
				card_p2 = _factory(abilities.first);
			}
			break;
		case 2:
			if (player == 0) {
				card_p1 = _factory(abilities.second);
			}
			else {
				card_p2 = _factory(abilities.second);
			}
			break;
		default:
			std::cout << "Invalid input";
			break;
		}
	}

	std::pair<MageTypeAbility, MageTypeAbility> MageService::_getAbilityForType(MageType type) {
		switch (type)
		{
			using enum MageTypeAbility;
		case MageType::Fire:
			return { Burn, BurnRowOrColumn };

		case MageType::Earth:
			return { Bury, Hole };

		case MageType::Air:
			return { BlowAway, BlowEter };

		case MageType::Water:
			return { Boat, BoatRowOrColumn };

		default:
			return { None, None };
		}
	}

	MageType MageService::_getTypeChoice(uint16_t choice) {
		switch (choice)
		{
		case 0:
			return MageType::Fire;

		case 1:
			return MageType::Earth;

		case 2:
			return MageType::Air;

		case 3:
			return MageType::Water;

		default:
			return MageType::None;
		}
	}

	std::unique_ptr<MageCard> MageService::_factory(MageTypeAbility ability_type) {
		switch (ability_type) {
			using enum MageTypeAbility;
		case Burn:
			return std::make_unique<MasterOfFireFront>();
			
		case BurnRowOrColumn:
			return std::make_unique<MasterOfFireBack>();
			
		case Bury:
			return std::make_unique<MasterOfEarthFront>();

		case Hole:
			return std::make_unique<MasterOfEarthBack>();

		case BlowAway:
			return std::make_unique<MasterOfAirFront>();

		case BlowEter:
			return std::make_unique<MasterOfAirBack>();

		case Boat:
			return std::make_unique<MasterOfWaterBack>();

		case BoatRowOrColumn:
			return std::make_unique<MasterOfWaterBack>();

		default:
			return nullptr;
		}
	}
}
