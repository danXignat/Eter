#pragma once
#include "Board.h"
#include"Player.h"
#include "PowerCard.h"
#include"IllusionService.h"
#include"BaseGameMode.h"

#include<memory>
#include"typedefs.h"
#include "logger.h"
#include<ranges> 
#include <algorithm>
#include<map>
#include<set>
namespace base {

	class ControllerExplosion :public PowerCard {
	public:
		ControllerExplosion(Board& board, Player& red, Player& blue);

		void apply() override;

	};

	class Destruction :public PowerCard {
	public:
		Destruction(Board& board, Player& red, Player& blue);

		bool canUseAbility(const Board& board, const Player& player) const;
		std::optional<Coord> getTargetPosition(const Board& board) const;
		void apply() override;
		std::string getErrorMessage(const Board& board, const Player& player) const;

	};

	class Flame :public PowerCard {
	public:
		Flame(Board& board, Player& red, Player& blue);

		void apply() override;

	};

	class Fire :public PowerCard {
	public:
		Fire(Board& board, Player& red, Player& blue);

		bool canUseAbility(const Board& board) const;
		std::vector<std::pair<Coord, CombatCardType>> getVisibleCards(const Board& board) const;
		std::vector<CombatCardType> getValidChoices(const Board& board) const;
		bool isValidChoice(CombatCardType chosen_type, const std::vector<CombatCardType>& valid_choices) const;
		void applyEffect(Board& board, CombatCardType chosen_type);
		std::string getErrorMessage(const Board& board) const;

		void apply() override;
		void setChosenCard(CombatCardType card_type);
	private:
		CombatCardType m_chosen_card;
		bool m_has_choice{ false };


	};

	class Ash :public PowerCard {
	public:
		Ash(Board& board, Player& red, Player& blue);
		struct UsedCardInfo {
			CombatCardType type;
			color::ColorType color;
		};

		bool canUseAbility(const Player& player) const;
		std::vector<UsedCardInfo> getUsedCardsInfo(const Player& player) const;


		std::string getErrorMessage(const Player& player) const;
		void setSelection(const Coord& coordinates, CombatCardType card_type);
		void apply() override;

	private:
		Coord m_selected_coord;
		CombatCardType m_selected_card;
		bool m_has_selection{ false };
	};

	class Spark :public PowerCard {
	public:
		Spark(Board& board, Player& red, Player& blue);
		void apply() override;
		std::vector<std::pair<Coord, CombatCardType>> coverCards(const Board& board, const Player& player);

		void setAvailableChoices(const std::vector<std::pair<Coord, CombatCardType>>& choices);
		std::optional<Coord> getSelectedFromCoord() const;
		void setSelectedFromCoord(Coord coord);

		void setSelectedCardType(CombatCardType type);
		std::optional<CombatCardType> getSelectedCardType() const;

		void setMoveDestination(Coord coord);
		std::optional<Coord> getMoveDestination() const;

	private:
		std::vector<std::pair<Coord, CombatCardType>> availableChoices;
		std::optional<Coord> selectedFromCoord;
		std::optional<CombatCardType> selectedCardType;
		std::optional<Coord> moveDestination;
	};



	class Squall :public PowerCard {
	public:


		Squall(Board& board, Player& red, Player& blue);

		void apply() override;
		std::vector<std::pair<Coord, CombatCardType>> getVisibleCards(const Board& board, const Player& player);

		void setVisibleCards(const std::vector<std::pair<Coord, CombatCardType>>& cards);
		std::optional<Coord> getSelectedCardCoord() const;
		void setSelectedCardCoord(Coord coord);

	private:
		std::vector<std::pair<Coord, CombatCardType>> visibleCards;
		std::optional<Coord> selectedCardCoord;


	};

	class Gale :public PowerCard {
	public:

		Gale(Board& board, Player& red, Player& blue);
		void apply() override;

	};

	class Hurricane :public PowerCard {
	public:
		Hurricane();
		void apply() override;

		void setOptions(const std::unordered_map<Orientation, std::vector<uint16_t>>& opts);
		std::unordered_map<Orientation, std::vector<uint16_t>> getOptions() const;

		void setUserSelection(Orientation orient, uint16_t index);
		std::optional<std::tuple<Orientation, uint16_t>> getUserSelection() const;

		void setUserDirection(MoveDirection dir);
		std::optional<MoveDirection> getUserDirection() const;

	private:
		std::unordered_map<Orientation, std::vector<uint16_t>> options;
		std::optional<std::tuple<Orientation, uint16_t>> userSelection;
		std::optional<MoveDirection> userDirection;
	};

	class Gust :public PowerCard {
	public:
		Gust(Board& board, Player& red, Player& blue);

		void apply() override;

		void setValidSourceCards(const std::vector<Coord>& sources);
		std::vector<Coord> getValidSourceCards() const;

		void setSelectedSource(Coord coord);
		std::optional<Coord> getSelectedSource() const;

		void setValidDestinations(const std::vector<Coord>& destinations);
		std::vector<Coord> getValidDestinations() const;

		void setSelectedDestination(Coord coord);
		std::optional<Coord> getSelectedDestination() const;

	private:
		std::vector<Coord> validSourceCards;
		std::optional<Coord> selectedSource;
		std::vector<Coord> validDestinations;
		std::optional<Coord> selectedDestination;
	};

	class Mirrage :public PowerCard {
	public:
		Mirrage(Board& board, Player& red, Player& blue);

		void apply() override;
		bool getIllusion(Board& board, Player& player);

	};

	class Storm :public PowerCard {
	public:
		Storm(Board& board, Player& red, Player& blue);
		void apply() override;
		void setAvailableStacks(const std::vector<Coord>& stacks);
		std::vector<Coord> getAvailableStacks() const;

		void setSelectedStack(size_t index);
		std::optional<size_t> getSelectedStack() const;

	private:
		std::vector<Coord> availableStacks;
		std::optional<size_t> selectedStack;
	};

	class Tide :public PowerCard {
	public:
		Tide(Board& board, Player& red, Player& blue);

		void apply() override;
		void setAvailableStacks(const std::vector<Coord>& stacks);
		std::vector<Coord> getAvailableStacks() const;

		void setSelectedStacks(const std::pair<Coord, Coord>& stacks);
		std::optional<std::pair<Coord, Coord>> getSelectedStacks() const;

	private:
		std::vector<Coord> availableStacks;
		std::optional<std::pair<Coord, Coord>> selectedStacks;

		std::vector<Coord> getStacks(const Board& board);

	};

	class Mist :public PowerCard {
	public:
		Mist(Board& board, Player& red, Player& blue);

		void apply() override;
		bool hasIllusion(Board& board, IllusionService& illusionService, Player& player);

	};

	class Wave :public PowerCard {
	public:
		Wave(Board& board, Player& red, Player& blue);

		void apply() override;
		void setAvailableStacks(const std::vector<Coord>& stacks);
		std::vector<Coord> getAvailableStacks() const;

		void setSelectedStack(const Coord& stack);
		std::optional<Coord> getSelectedStack() const;

		void setDestination(const Coord& destination);
		std::optional<Coord> getDestination() const;

		void setSelectedCard(char card);
		std::optional<char> getSelectedCard() const;

	private:
		std::vector<Coord> availableStacks;
		std::optional<Coord> selectedStack;
		std::optional<Coord> destination;
		std::optional<char> selectedCard;

		std::vector<Coord> validStacks(const Board& board) const;
	};


	class Whirlpool : public PowerCard {
	public:
		Whirlpool(Board& board, Player& red, Player& blue);
		void apply() override;
		void setUserChoice(char choice);
		char getUserChoice() const;

		void setFirstCoord(const Coord& coord);
		Coord getFirstCoord() const;

		void setSecondCoord(const Coord& coord);
		Coord getSecondCoord() const;

		void setEqualCardCoord(const Coord& coord);
		Coord getEqualCardCoord() const;

		void setValidPairsOnRow(const std::vector<std::pair<Coord, Coord>>& pairs);
		std::vector<std::pair<Coord, Coord>> getValidPairsOnRow() const;

		void setValidPairsOnColumn(const std::vector<std::pair<Coord, Coord>>& pairs);
		std::vector<std::pair<Coord, Coord>> getValidPairsOnColumn() const;

	private:
		std::vector<std::pair<Coord, Coord>> validPairsOnRow;
		std::vector<std::pair<Coord, Coord>> validPairsOnColumn;
		char userChoice;
		Coord firstCoord, secondCoord, equalCardCoord;

		std::vector<std::pair<Coord, Coord>> getPairsOnRow(Board& board);
		std::vector<std::pair<Coord, Coord>> getPairsOnColumn(Board& board);
	};


	class Blizzard :public PowerCard {
	public:
		Blizzard(Board& board, Player& red, Player& blue);
		void setBlockChoice(char choice);
		std::optional<char> getBlockChoice() const;

		void setChosenRow(uint16_t row);
		std::optional<uint16_t> getChosenRow() const;

		void setChosenColumn(uint16_t column);
		std::optional<uint16_t> getChosenColumn() const;
		void apply() override;
	private:
		std::optional<char> blockChoice;
		std::optional<uint16_t> chosenRow;
		std::optional<uint16_t> chosenColumn;


	};

	class Waterfall :public PowerCard {
	public:
		Waterfall(Board& board, Player& red, Player& blue);
		void setOrientation(Orientation orientation);
		std::optional<Orientation> getOrientation() const;

		void setIndex(int16_t index);
		std::optional<int16_t> getIndex() const;

		void setMoveDirection(MoveDirection direction);
		std::optional<MoveDirection> getMoveDirection() const;

		std::unordered_map<Orientation, std::vector<int16_t>> getOptions(Board& board);
		std::tuple<Orientation, int16_t, MoveDirection> input(Board& board);
		void apply() override;

	private:
		std::optional<Orientation> selectedOrientation;
		std::optional<int16_t> selectedIndex;
		std::optional<MoveDirection> selectedMoveDirection;

	};

	class Support :public PowerCard {
	public:
		Support(Board& board, Player& red, Player& blue);

		void apply() override;
		void setSelectedCoord(const Coord& coord);
		std::optional<Coord> getSelectedCoord() const;
		std::vector<Coord> CoordCardType(Board& board, const Player& player) const;
	private:
		std::optional<Coord> selectedCoord;

	};

	class Earthquake :public PowerCard {
	public:
		Earthquake(Board& board, Player& red, Player& blue);

		void apply() override;
		void setCardsToRemove(const std::vector<Coord>& cards);
		std::vector<Coord> getCardsToRemove() const;
	private:
		std::vector<Coord> cardsToRemove;


	};

	class Crumble :public PowerCard {
	public:
		Crumble(Board& board, Player& red, Player& blue);

		void apply() override;
		void setValidCards(const std::vector<Coord>& cards);
		std::vector<Coord> getValidCards() const;
		std::vector<Coord> findValidCards(const Board& board, const Player& player) const;
		void setSelectedCard(const Coord& coord);
		std::optional<Coord> getSelectedCard() const;
	private:
		std::vector<Coord> validCards;
		std::optional<Coord> selectedCard;

	};

	class Border :public PowerCard {
	public:
		Border(Board& board, Player& red, Player& blue);

		void apply() override;
		void setValidPositions(const std::set<int>& rows, const std::set<int>& cols);
		std::pair<std::set<int>, std::set<int>> getValidPositions() const;

		void setSelectedCoord(const Coord& coord);
		std::optional<Coord> getSelectedCoord() const;

		void setSelectedCardType(CombatCardType type);
		std::optional<CombatCardType> getSelectedCardType() const;

	private:
		bool applyNeutralCard(Player& player, Board& board);

		std::set<int> validRows;
		std::set<int> validCols;
		std::optional<Coord> selectedCoord;
		std::optional<CombatCardType> selectedCardType;
	};


	class Avalanche :public PowerCard {
	public:
		Avalanche(Board& board, Player& red, Player& blue);

		void apply() override;
		std::vector<std::pair<Orientation, std::pair<Coord, Coord>>> getPairs(Board& board);
		std::vector<std::pair<MoveDirection, std::pair<Coord, Coord>>> checkShifting(
			const std::vector<std::pair<Orientation, std::pair<Coord, Coord>>>& pack, Board& board);

		void setSelectedMove(const std::pair<MoveDirection, std::pair<Coord, Coord>>& move);
		std::optional<std::pair<MoveDirection, std::pair<Coord, Coord>>> getSelectedMove() const;
	private:
		std::optional<std::pair<MoveDirection, std::pair<Coord, Coord>>> selectedMove;


	};

	class Rock :public PowerCard {
	public:
		Rock(Board& board, Player& red, Player& blue);

		void apply() override;

		std::vector<Coord>getIllusionCoords(const Board& board) const;

	};
}