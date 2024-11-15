import MageDuelMode;
import MageCardType;
import MageCardTypeAbility;
import<random>;
#include <algorithm>
#include <stdexcept>

namespace base {

	MageDuelMode::MageDuelMode(std::string Player1, std::string Player2) :
		TrainingMode(Player1, Player2)

	{
		m_board = Board{4};
	    m_win_manager = WinManager{ 4 };
	    initializeMageCards();
		assignRandomCardToPlayers();
	}

	void MageDuelMode::initializeMageCards() {

		using enum MageCardTypeAbility;
		using enum MageCardType;
		mageCardsPool.emplace_back(std::make_unique<MageCard>(Fire, Burn, BurnRowOrColumn));
		mageCardsPool.emplace_back(std::make_unique<MageCard>(Earth, Bury, Hole));
		mageCardsPool.emplace_back(std::make_unique<MageCard>(Air, BlowAway, BlowEter));
		mageCardsPool.emplace_back(std::make_unique<MageCard>(Water, Boat, BoatRowOrColumn));
		
	}

	void MageDuelMode::assignRandomCardToPlayers() {
		std::random_device randomDev;
		std::mt19937 gen(randomDev());
		std::shuffle(mageCardsPool.begin(), mageCardsPool.end(), gen);
		if (!mageCardsPool.empty()) {
			m_player1.addMageCard(std::move(mageCardsPool.back()));
			mageCardsPool.pop_back();
		}
		if (!mageCardsPool.empty()) {
			m_player2.addMageCard(std::move(mageCardsPool.back()));
			mageCardsPool.pop_back();
		}
	}

}