#include "MageService.h"

#include <memory>
#include <random>
#include <format>

namespace base {
	MageService::MageService(Board& board, Player& red, Player& blue) :
		m_board{ board },
		m_player_red{red},
		m_player_blue{blue} {
		std::random_device rd;
		std::mt19937 gen(rd());

		std::array<uint16_t, MAGE_NUMBER> choices;
		std::iota(choices.begin(), choices.end(), 0);
		std::shuffle(choices.begin(), choices.end(), gen);

		MageTypeAbility choice_p1 = static_cast<MageTypeAbility>(choices[0]);
		MageTypeAbility choice_p2 = static_cast<MageTypeAbility>(choices[1]);

		/*choice_p1 = static_cast<MageTypeAbility>(1);
		choice_p2 = static_cast<MageTypeAbility>(1);*/

		card_red  = _factory(choice_p1, color::ColorType::RED);
		card_blue = _factory(choice_p2, color::ColorType::BLUE);
	}
	
	MageTypeAbility MageService::getMageAbility(color::ColorType color) {
		if (color == color::ColorType::RED) {
			return card_red->getMasterType();
		}
		else {
			return card_blue->getMasterType();
		}
	}

	bool MageService::apply(color::ColorType color) {

		if (color == color::ColorType::RED) {

			if (card_red) {
				if (card_red->apply()) {
					card_red.reset();
					return true;
				}
				else
					return false;
			}
			else {
				return false;
			}

		}
		else {

			if (card_blue) {
				if (card_blue->apply()) {
					card_blue.reset();
					return true;
				}
				else {
					return false;
				}
			}
			else {
				return false;
			}

		}
	}

	void MageService::renderCard(PlayerRef player) const {
		if (player.get().getColor() == color::ColorType::RED && card_red) {
			utils::printAtCoordinate(
				std::format("Mage << {} >> {}", masterToString(card_red->getMasterType()), getMasterDescription(card_red->getMasterType())),
				1, 90
			);
		}
		else if (card_blue){
			utils::printAtCoordinate(
				std::format("Mage << {} >> {}", masterToString(card_blue->getMasterType()), getMasterDescription(card_blue->getMasterType())),
				1, 90
			);
		}
	}

	std::unique_ptr<MageCard> MageService::_factory(MageTypeAbility ability_type, color::ColorType color) {
		switch (ability_type) {
			using enum MageTypeAbility;
			
		case Burn:
			return std::make_unique<MasterOfFireFront>	(m_board, m_player_red, m_player_blue, color);
			
		case BurnRowOrColumn:
			return std::make_unique<MasterOfFireBack>	(m_board, m_player_red, m_player_blue, color);
			
		case Bury:
			return std::make_unique<MasterOfEarthFront>	(m_board, m_player_red, m_player_blue, color);

		case Hole:
			return std::make_unique<MasterOfEarthBack>	(m_board, m_player_red, m_player_blue, color);

		case BlowAway:
			return std::make_unique<MasterOfAirFront>	(m_board, m_player_red, m_player_blue, color);

		case BlowEter:
			return std::make_unique<MasterOfAirBack>	(m_board, m_player_red, m_player_blue, color);

		case Boat:
			return std::make_unique<MasterOfWaterFront>	(m_board, m_player_red, m_player_blue, color);

		case BoatRowOrColumn:
			return std::make_unique<MasterOfWaterBack>	(m_board, m_player_red, m_player_blue, color);

		default:
			return nullptr;
		}
	}
}
