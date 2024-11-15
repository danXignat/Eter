export module MageDuelMode;
import TrainingMode;
import MageCard;

namespace base {

	export class MageDuelMode : public TrainingMode {

	private:

		std::vector<std::unique_ptr<MageCard>>mageCardsPool;

	public:

		MageDuelMode(std::string Player1, std::string Player2);

		void initializeMageCards();
		void assignRandomCardToPlayers();
	};
}