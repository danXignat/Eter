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
		static const std::vector<PowerCardType> explosion_options;			
		static const std::vector<PowerCardType> illusion_options;

		ElementalService() = default;
		ElementalService(Board& board, Player& red_player, Player& blue_player
			, std::optional<IllusionService>& illusion_service, std::optional<ExplosionService>& explosion_service);

		void apply(char choice, Player& player);
		void renderCards() const;
		void pickRandomCards();
		
		std::pair<std::pair<uint16_t, PowerCardType>, std::pair<uint16_t, PowerCardType>> getCardsData() const;

		template <typename T>
		T* getElementalCard(uint16_t id);

	public:
		static constexpr const uint16_t power_number = 24;

	private:
		std::unique_ptr<PowerCard>_factory(PowerCardType ability);

	private:
		Board& m_board;
		Player& m_red_player, &m_blue_player;

		PowerCardType type_card1;
		PowerCardType type_card2;

		std::unordered_set<uint16_t> m_choices;

		std::unique_ptr<PowerCard>card1;
		std::unique_ptr<PowerCard>card2;

		std::optional <IllusionService>& m_illusion_service;
		std::optional <ExplosionService>& m_explosion_service;
	};

	template <typename T>
	T* ElementalService::getElementalCard(uint16_t id) {
		if (card1->getId() == id) {
			return dynamic_cast<T*>(card1.get());
		}
		else if (card2->getId() == id) {
			return dynamic_cast<T*>(card2.get());
		}

		return nullptr;
	}
}