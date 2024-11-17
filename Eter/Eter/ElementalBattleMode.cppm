export module ElementalBattleMode;
import TrainingMode;
import ElementalPowerCard;

namespace base {
	export class ElementalBattleMode :public TrainingMode {
	public:
		ElementalBattleMode(std::string player1, std::string player2);

		void randomPowerCards();

	private:
		std::vector < std::unique_ptr<ElementalPowerCard>>m_ElementalCards;
	};
}