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
		ElementalService() = default;
		ElementalService(Board& board);
		void apply(char choice, Player& player);
		void renderCards() const;

	public:
		static constexpr const uint16_t power_number = 24;

	private:
		std::unique_ptr<PowerCard>_factory(PowerCardType ability);
		PowerCardType _getPowerCard(PowerCardType type);

	private:
		Board& m_board;

		PowerCardType type_card1;
		PowerCardType type_card2;

		std::unique_ptr<PowerCard>card1;
		std::unique_ptr<PowerCard>card2;

	};
}