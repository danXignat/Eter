#include "MageService.h"

#include <memory>
#include <random>
#include <format>

namespace base {
	MageService::MageService(Board& board) : m_board{ board } {
		std::random_device rd;
		std::mt19937 gen(rd());

		std::array<uint16_t, MageService::mage_number> choices;
		std::iota(choices.begin(), choices.end(), 0);
		std::shuffle(choices.begin(), choices.end(), gen);

		MageTypeAbility choice_p1 = static_cast<MageTypeAbility>(choices[0]);
		MageTypeAbility choice_p2 = static_cast<MageTypeAbility>(choices[1]);

		m_card_p1 = _factory(MageTypeAbility::Boat);
		m_card_p2 = _factory(MageTypeAbility::Boat);

		/*m_card_p1 = _factory(choice_p1);
		m_card_p2 = _factory(choice_p2);*/
	}
	
	bool MageService::apply(Player& player) {

		if (player.getColor() == color::ColorType::RED) {

			if (m_card_p1) {
				m_card_p1->apply(m_board, player);
				m_card_p1.reset();
				return true;
			}
			else {
				return false;
			}

		}
		else {

			if (m_card_p2) {
				m_card_p2->apply(m_board, player);
				m_card_p2.reset();
				return true;
			}
			else {
				return false;
			}

		}
	}

	void MageService::renderCard(PlayerRef player) const {
		if (player.get().getColor() == color::ColorType::RED && m_card_p1) {
			utils::printAtCoordinate(
				std::format("Mage << {} >> {}", masterToString(m_card_p1->getMasterType()), getMasterDescription(m_card_p1->getMasterType())),
				1, 90
			);
		}
		else if (m_card_p2){
			utils::printAtCoordinate(
				std::format("Mage << {} >> {}", masterToString(m_card_p2->getMasterType()), getMasterDescription(m_card_p2->getMasterType())),
				1, 90
			);
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
			return std::make_unique<MasterOfWaterFront>();

		case BoatRowOrColumn:
			return std::make_unique<MasterOfWaterBack>();

		default:
			return nullptr;
		}
	}
}
