//#include "MageService.h"
//#include<memory>
//#include <random>
//namespace base {
//	MageService::MageService(){
//		m_mage_cards_pool.push_back(std::make_unique< MasterOfFire>());
//		m_mage_cards_pool.push_back(std::make_unique< MasterOfWater> ());
//		m_mage_cards_pool.push_back(std::make_unique< MasterOfAir> ());
//		m_mage_cards_pool.push_back(std::make_unique< MasterOfEarth> ());
//	}
//	
//	void MageService::generateRandomCard(Player & player) {
//			std::random_device rd;
//			std::mt19937 gen(rd());
//			std::uniform_int_distribution<> dis(0, m_mage_cards_pool.size() - 1);
//			int randomIndex = dis(gen);
//			std::unique_ptr<MageCard> selectedCard = std::move(m_mage_cards_pool[randomIndex]);
//			player.setMageCard(std::move(selectedCard));
//	}
//}
