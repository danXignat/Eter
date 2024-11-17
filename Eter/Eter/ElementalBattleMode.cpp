import ElementalBattleMode;
import ElementalPowerCardType;
import<random>;
//import<algorithm>;
//import<stdexcept>;

namespace base {
	ElementalBattleMode::ElementalBattleMode(std::string Player1, std::string Player2) :
        TrainingMode(Player1, Player2)

    {
        m_board = Board{ 4 };
        m_win_manager = WinManager{ 4 };
        randomPowerCards();
    }

    void ElementalBattleMode::randomPowerCards() {

        for (const auto& [cardType, cardName] : mapPowers) {
            m_ElementalCards.push_back(std::make_unique<ElementalPowerCard>(cardType));
        }
        std::random_device randomDev;
        std::mt19937 gen(randomDev());
        std::shuffle(m_ElementalCards.begin(), m_ElementalCards.end(), gen);

        if (!m_ElementalCards.empty()) {
            m_player1.addPowerCard(std::move(m_ElementalCards.back()));
            m_ElementalCards.pop_back();
        }

        if (!m_ElementalCards.empty()) {
            m_player2.addPowerCard(std::move(m_ElementalCards.back()));
            m_ElementalCards.pop_back();
        }

 
    }
}

