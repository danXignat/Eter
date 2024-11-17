export module MageDuelMode;
import TrainingMode;
import MageCard;

namespace base {

	export class MageDuelMode : public TrainingMode {

	public:

		MageDuelMode(std::string Player1, std::string Player2);

		void initializeMageCards();
		void assignRandomCardToPlayers();
		
		void removeOpponentCard(const Coord& coord, Player& player);
		void removeRow(const Coord& startCoord, Player& player);
		void removeColumn(const Coord& start_coord, Player& player);

	private:

		std::vector<std::unique_ptr<MageCard>>m_mageCardsPool;
	};
}