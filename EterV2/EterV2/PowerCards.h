#pragma once
#include "Board.h"
#include"Player.h"
#include "PowerCard.h"
#include"IllusionService.h"
#include"BaseGameMode.h"
#include"typedefs.h"
#include "logger.h"

#include <memory>
#include <vector>
#include <ranges> 
#include <algorithm>
#include <map>
#include <set>

namespace base {

	class ControllerExplosion :public PowerCard {
	public:
		ControllerExplosion(Board& board, Player& red, Player& blue);

		void apply() override;

	};

	class Destruction :public PowerCard {
	public:
		Destruction(Board& board, Player& red, Player& blue);

		bool canUseAbility(color::ColorType color) const;

		uint16_t getTargetCardID() const;
		std::string getErrorMessage(color::ColorType colorPlayer) const;

		void setColor(color::ColorType colorPlayer);

		void apply() override;

	private:
		color::ColorType m_color = color::ColorType::DEFAULT;
	};

	class Flame :public PowerCard {
	public:
		Flame(Board& board, Player& red, Player& blue);

		void apply() override;

	};

	class Fire :public PowerCard {
	public:
		Fire(Board& board, Player& red, Player& blue);

		std::vector<std::pair<Coord, CombatCardType>> getVisibleCards() const;
		std::vector<CombatCardType> getValidChoices() const;
		std::string getErrorMessage() const;
		std::unordered_set<uint16_t> getVisibleCardIDs() const;
		std::unordered_set<uint16_t> getCardIDsOfType(CombatCardType type) const;
		std::optional<CombatCardType> getCardTypeByID(uint16_t card_id) const;

		void setChosenCardByID(uint16_t card_id);
		void setChosenCard(CombatCardType card_type);

		bool canUseAbility() const;
		bool isValidChoice(CombatCardType chosen_type, const std::vector<CombatCardType>& valid_choices) const;

		void applyEffect(CombatCardType chosen_type); 
		void apply() override;
		

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
		

		bool canUseAbility(color::ColorType colorPlayer) const;
		
		std::vector<UsedCardInfo> getUsedCardsInfo(color::ColorType colorPlayer) const;
		std::string getErrorMessage(color::ColorType color) const;
		std::unordered_set<uint16_t> getUsedCardIDs(color::ColorType colorPlayer) const;
		std::optional<std::pair<CombatCardType, color::ColorType>> getCardInfoByID(uint16_t card_id) const;

		void setSelectionByID(const Coord& coordinates, uint16_t card_id);
		void setSelection(const Coord& coordinates, CombatCardType card_type);
		void setColor(color::ColorType colorPlayer);

		void apply() override; 

	private:
		color::ColorType m_color = color::ColorType::DEFAULT;
		Coord m_selected_coord;
		CombatCardType m_selected_card;
		bool m_has_selection{ false };
	};

	class Spark :public PowerCard {
	public:
		Spark(Board& board, Player& red, Player& blue);

		struct CardInfo {
			Coord coord;
			CombatCardType type;
			uint16_t id;
		};

		std::unordered_set<uint16_t> getCoveredCardIDs();
		std::optional<Coord> getSelectedFromCoord() const;
		std::optional<Coord> getMoveDestination() const;
		std::optional<CombatCardType> getSelectedCardType() const; 
		std::optional<CardInfo> getCardInfoByID(uint16_t card_id) const;

		std::vector<std::pair<Coord, CombatCardType>> coverCards();

		void setSelectionByID(uint16_t card_id);
		void setAvailableChoices(const std::vector<std::pair<Coord, CombatCardType>>& choices);
		void setSelectedFromCoord(Coord coord);
		void setSelectedCardType(CombatCardType type);
		void setMoveDestination(Coord coord);
		void setColor(color::ColorType colorPlayer);

		void apply() override;

	private:
		color::ColorType m_color = color::ColorType::DEFAULT;

		std::vector<std::pair<Coord, CombatCardType>> availableChoices;
		std::optional<Coord> selectedFromCoord;
		std::optional<CombatCardType> selectedCardType;
		std::optional<Coord> moveDestination;
	};



	class Squall :public PowerCard {
	public:


		Squall(Board& board, Player& red, Player& blue);

		void apply() override;
		std::vector<std::pair<Coord, CombatCardType>> getVisibleCards();
		std::unordered_set<uint16_t> getVisibleCardsIDs() const;
		std::optional<CombatCardType> getCardTypeByID(uint16_t card_id) const;
		std::optional<uint16_t> getSelectedCardID() const;

		void setSelectedCardID(uint16_t card_id);
		void setVisibleCards(const std::vector<std::pair<Coord, CombatCardType>>& cards);
		std::optional<Coord> getSelectedCardCoord() const;
		void setSelectedCardCoord(Coord coord);

		void setColor(color::ColorType colorPlayer);

	private:
		color::ColorType m_color = color::ColorType::DEFAULT;
		std::vector<std::pair<Coord, CombatCardType>> visibleCards;
		std::optional<Coord> selectedCardCoord;
		std::optional<uint16_t> m_selected_card_id;


	};

	class Gale :public PowerCard {
	public:

		Gale(Board& board, Player& red, Player& blue);
		void apply() override;
		void setColor(color::ColorType colorPlayer);

	private:
		color::ColorType m_color = color::ColorType::DEFAULT;

	};

	class Hurricane :public PowerCard {
	public:
		Hurricane(Board& board, Player& red, Player& blue);
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

		
		std::vector<Coord> getValidSourceCards() const;
		std::unordered_set<uint16_t> getValidSourceCardIDs() const;
		std::unordered_set<uint16_t> getValidDestinationIDs() const;
		std::optional<uint16_t> getSelectedSourceID() const;
		std::optional<uint16_t> getSelectedDestinationID() const;
		std::optional<Coord> getSelectedSource() const;
		std::optional<Coord> getSelectedDestination() const;
		std::vector<Coord> getValidDestinations() const;

		void setValidDestinations(const std::vector<Coord>& destinations);
		void setValidSourceCards(const std::vector<Coord>& sources);
		void setSelectedSource(Coord coord);
		void setSelectedDestinationID(uint16_t card_id);
		void setSelectedSourceID(uint16_t card_id);
		void setSelectedDestination(Coord coord);
		

	private:
		std::vector<Coord> validSourceCards;
		std::optional<Coord> selectedSource;
		std::vector<Coord> validDestinations;
		std::optional<Coord> selectedDestination;
		std::optional<uint16_t> m_selected_source_id;
		std::optional<uint16_t> m_selected_destination_id;
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
		
		std::vector<Coord> getAvailableStacks() const;
		std::unordered_set<uint16_t> getAvailableStackIDs() const;
		std::optional<size_t> getStackSize(uint16_t card_id) const;
		std::optional<size_t> getSelectedStack() const;
		std::optional<uint16_t> getSelectedStackID() const;
		void setSelectedStackID(uint16_t card_id);
		void setSelectedStack(size_t index);
		void setAvailableStacks(const std::vector<Coord>& stacks);
		

	private:
		std::vector<Coord> availableStacks;
		std::optional<size_t> selectedStack;
		std::optional<uint16_t> m_selected_stack_id; 
	};

	class Tide :public PowerCard {
	public:
		Tide(Board& board, Player& red, Player& blue);

		void apply() override;

		std::vector<Coord> getAvailableStacks() const;
		std::optional<std::pair<Coord, Coord>> getSelectedStacks() const;
		std::unordered_set<uint16_t> getAvailableStackIDs() const; 
		std::optional<std::pair<uint16_t, uint16_t>> getSelectedStackIDs();

		void setSelectedStacks(const std::pair<Coord, Coord>& stacks);
		void setSelectedStackIDs(uint16_t first_id, uint16_t second_id);
		void setAvailableStacks(const std::vector<Coord>& stacks);

	private:
		std::vector<Coord> availableStacks;
		std::optional<std::pair<Coord, Coord>> selectedStacks;

		std::vector<Coord> getStacks();
		std::optional<std::pair<uint16_t, uint16_t>> m_selected_stack_ids;
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
		std::unordered_set<uint16_t> getAvailableStackIDs() const;
		void setSelectedStackID(uint16_t stack_id);
		std::optional<uint16_t> getSelectedStackID() const;
		void setDestination(const Coord& destination);
		std::optional<Coord> getDestination() const;

		void setSelectedCard(char card);
		std::optional<char> getSelectedCard() const;

		void setColor(color::ColorType colorPlayer);

	private:
		color::ColorType m_color = color::ColorType::DEFAULT;
		std::vector<Coord> m_available_stacks;
		std::optional<Coord> m_selected_stack;
		std::optional<Coord> m_destination;
		std::optional<char> m_selected_card;
		std::optional<uint16_t> m_selected_stack_id;
		std::vector<Coord> validStacks() const;
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

		std::vector<std::pair<Coord, Coord>> getPairsOnRow();
		std::vector<std::pair<Coord, Coord>> getPairsOnColumn();
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
		void setColor(color::ColorType colorPlayer);

	private:
		color::ColorType m_color = color::ColorType::DEFAULT;
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

		std::unordered_map<Orientation, std::vector<int16_t>> getOptions();
		std::tuple<Orientation, int16_t, MoveDirection> input();
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
		std::vector<Coord> CoordCardType( ) const;
		void setColor(color::ColorType colorPlayer);

	private:
		color::ColorType m_color = color::ColorType::DEFAULT;
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
		std::vector<Coord> findValidCards() const;
		void setSelectedCard(const Coord& coord);
		std::optional<Coord> getSelectedCard() const;
		void setColor(color::ColorType colorPlayer);

	private:
		color::ColorType m_color = color::ColorType::DEFAULT;
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

		void setColor(color::ColorType colorPlayer);

	private:
		color::ColorType m_color = color::ColorType::DEFAULT;
		bool applyNeutralCard();

		std::set<int> validRows;
		std::set<int> validCols;
		std::optional<Coord> selectedCoord;
		std::optional<CombatCardType> selectedCardType;
	};


	class Avalanche :public PowerCard {
	public:
		Avalanche(Board& board, Player& red, Player& blue);

		void apply() override;
		std::vector<std::pair<Orientation, std::pair<Coord, Coord>>> getPairs();
		std::vector<std::pair<MoveDirection, std::pair<Coord, Coord>>> checkShifting(
			const std::vector<std::pair<Orientation, std::pair<Coord, Coord>>>& pack);

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