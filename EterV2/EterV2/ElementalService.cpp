#include"ElementalService.h"

#include <cstdint>
#include <random>
#include <memory>
#include <algorithm>

#include "logger.h"
#include "utils.h"

using namespace logger;

namespace base {
	ElementalService::ElementalService(Board& board, Player& red_player, Player& blue_player) : m_board{ board },
		m_red_player{ red_player }, m_blue_player{ blue_player } {
		std::random_device rd;
		std::mt19937 gen(rd());

		std::array<uint16_t, ElementalService::power_number> choices;
		std::iota(choices.begin(), choices.end(), 0);
		std::shuffle(choices.begin(), choices.end(), gen);

		type_card1 = static_cast<PowerCardType>(choices[0]);
		type_card2 = static_cast<PowerCardType>(choices[1]);

		card1 = _factory(type_card1);
		card2 = _factory(type_card2);
	}

	void ElementalService::apply(char choice, Player& player) {
		switch (choice) {
		case '1': {
			card1->apply();
			card1.reset();
			break;
		}

		case '2': {
			card2->apply();
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
				1, 28
			);
		}

		if (card2) {
			utils::printAtCoordinate(
				std::format("Power 2 << {} >> {}", typeToStrings(type_card2), abilityToString(type_card2)),
				1, 29
			);
		}
	}

	std::unique_ptr<PowerCard>ElementalService::_factory(PowerCardType ability) {
		switch (ability) {
		case PowerCardType::ControllerExplosion:
			return std::make_unique<ControllerExplosion>(m_board, m_red_player, m_blue_player);

		case PowerCardType::Destruction:
			return std::make_unique<Destruction>(m_board, m_red_player, m_blue_player);

		case PowerCardType::Flame:
			return std::make_unique<Flame>(m_board, m_red_player, m_blue_player);

		case PowerCardType::Fire:
			return std::make_unique<Fire>(m_board, m_red_player, m_blue_player);

		case PowerCardType::Ash:
			return std::make_unique<Ash>(m_board, m_red_player, m_blue_player);

		case PowerCardType::Spark:
			return std::make_unique<Spark>(m_board, m_red_player, m_blue_player);

		case PowerCardType::Squall:
			return std::make_unique<Squall>(m_board, m_red_player, m_blue_player);

		case PowerCardType::Gale:
			return std::make_unique<Gale>(m_board, m_red_player, m_blue_player);

		case PowerCardType::Hurricane:
			return std::make_unique<Hurricane>(m_board, m_red_player, m_blue_player);

		case PowerCardType::Gust:
			return std::make_unique<Gust>(m_board, m_red_player, m_blue_player);

		case PowerCardType::Mirrage:
			return std::make_unique<Mirrage>(m_board, m_red_player, m_blue_player);

		case PowerCardType::Storm:
			return std::make_unique<Storm>(m_board, m_red_player, m_blue_player);

		case PowerCardType::Tide:
			return std::make_unique<Tide>(m_board, m_red_player, m_blue_player);

		case PowerCardType::Mist:
			return std::make_unique<Mist>(m_board, m_red_player, m_blue_player);

		case PowerCardType::Wave:
			return std::make_unique<Wave>(m_board, m_red_player, m_blue_player);

		case PowerCardType::Whirlpool:
			return std::make_unique<Whirlpool>(m_board, m_red_player, m_blue_player);

		case PowerCardType::Blizzard:
			return std::make_unique<Blizzard>(m_board, m_red_player, m_blue_player);

		case PowerCardType::Waterfall:
			return std::make_unique<Waterfall>(m_board, m_red_player, m_blue_player);

		case PowerCardType::Support:
			return std::make_unique<Support>(m_board, m_red_player, m_blue_player);

		case PowerCardType::Earthquake:
			return std::make_unique<Earthquake>(m_board, m_red_player, m_blue_player);

		case PowerCardType::Crumble:
			return std::make_unique<Crumble>(m_board, m_red_player, m_blue_player);

		case PowerCardType::Border:
			return std::make_unique<Border>(m_board, m_red_player, m_blue_player);

		case PowerCardType::Avalanche:
			return std::make_unique<Avalanche>(m_board, m_red_player, m_blue_player);

		case PowerCardType::Rock:
			return std::make_unique<Rock>(m_board, m_red_player, m_blue_player);

		default:
			return nullptr;
		}
	}

	std::pair<std::pair<uint16_t, PowerCardType>, std::pair<uint16_t, PowerCardType>> ElementalService::getCardsData() const {
		return { {card1->getId(), card1->getType()},
			{card2->getId(), card2->getType()} };
	}

}