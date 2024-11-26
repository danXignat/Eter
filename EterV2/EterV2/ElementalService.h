#pragma once

#include <vector> 
#include <memory>
#include <array>
#include <numeric>
#include <algorithm>


#include"PowerCard.h"
#include"PowerCardType.h"
#include"PowerCards.h"
#include"Player.h"
namespace base {
	class ElementalService {
	public:
		static constexpr const int power_number = 24;

		ElementalService(Board& board);
		void selectPowerCards();
		void apply(Player& player);

	private:
		std::unique_ptr<PowerCard>_factory(PowerCardType ability);
		PowerCardType _getPowerCard(PowerCardType type);
		void _setPlayerPowerCard(bool player);

	private:
		Board& m_board;
		PowerCardType type_p1;
		PowerCardType type_p2;

		std::unique_ptr<PowerCard>card_p1;
		std::unique_ptr<PowerCard>card_p2;

	};
}