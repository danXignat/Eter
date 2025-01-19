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

namespace base {

	class ControllerExplosion :public PowerCard {
	public:
		ControllerExplosion();

		void apply(Board& board, Player& player) override;

	};

	class Destruction :public PowerCard {
	public:
		Destruction();
		void apply( Board& board, Player& player) override;

	};

	class Flame :public PowerCard {
	public:
		Flame();

		void apply(Board& board, Player& player) override;
		void setSelectedMove(const Coord& coord, CombatCardType cardType);
		const std::vector<std::pair<Coord, CombatCardType>>& getAvailableChoices() const;


	private:
		bool m_hasUserSelected;
		Coord m_selectedCoord;
		CombatCardType m_selectedCardType;
		std::vector<std::pair<Coord, CombatCardType>> m_availableChoices;

	};

	class Fire :public PowerCard {
	public:
		Fire()=default;

		Fire(Player& red_player, Player& blue_player);

		void apply(Board& board, Player& player) override;
		std::vector<Coord>getDuplicateCards(Board& board, const Player& player);

	private:
		Player& m_red_player;
		Player& m_blue_player;

	};

	class Ash :public PowerCard {
	public:
		Ash();

		void apply(Board& board, Player& player) override;
		void setSelectedMove(const Coord& coord, CombatCardType cardType);

	private:
		bool m_hasUserSelected;
		Coord m_selectedCoord;
		CombatCardType m_selectedCardType;

	};

	class Spark:public PowerCard {
	public:
		Spark();

		void apply(Board& board, Player& player) override;
		std::vector<std::pair<Coord, CombatCardType>> coverCards(const Board& board, const Player& player);
		void setSelectedMove(const Coord& from, const Coord& to, CombatCardType cardType);
		const std::vector<std::pair<Coord, CombatCardType>>& getAvailableChoices() const;

	private:
		bool m_hasUserSelected;
		Coord m_coordFrom;
		Coord m_coordTo;
		CombatCardType m_selectedCardType;
		std::vector<std::pair<Coord, CombatCardType>> m_availableChoices;
	};

	class Squall :public PowerCard {
	public:

		Squall() = default;
		Squall(Player& red_player, Player& blue_player);
		void apply(Board& board, Player& player) override;
		void setSelectedMove(const Coord& coord);

	private:
		bool m_hasUserSelected;
		Coord m_selectedCoord;
		Player& m_red_player, & m_blue_player;
	};

	class Gale :public PowerCard {
	public:
		Gale() = default;
		Gale(Player& red_player, Player& blue_player);

		void apply(Board& board, Player& player) override;


	private:
		Player& m_red_player;
		Player& m_blue_player;
	};

	class Hurricane :public PowerCard {
	public:
		Hurricane();
		
		void apply(Board& board, Player& player) override;
		const std::unordered_map<Orientation, std::vector<uint16_t>>& getOptions(Board& board);
		void setSelectedMove(Orientation orientation, uint16_t index, MoveDirection direction);


	private:
		bool m_hasUserSelected;
		Orientation m_selectedOrientation;
		uint16_t m_selectedIndex;
		MoveDirection m_selectedDirection;
		std::unordered_map<Orientation, std::vector<uint16_t>> m_options;


	};

	class Gust :public PowerCard {
	public:
		Gust();

		void apply(Board& board, Player& player) override;
		std::vector<Coord>getCardsCoord(const Board& board)const;
		void setSelectedMove(const Coord& fromCoord, const Coord& toCoord);
		const std::vector<Coord>& getAvailableCards() const;
		const std::vector<Coord>& getValidMoves(const Coord& selectedCard, Board& board);
		std::vector<Coord> getCardsCoord(const Board& board);


	private:
		bool m_hasUserSelected;
		Coord m_selectedFromCoord;
		Coord m_selectedToCoord;
		std::vector<Coord> m_availableCards;
		std::vector<Coord> m_validMoves;

	};

	class Mirrage :public PowerCard {
	public:
		Mirrage();

		void apply(Board& board, Player& player) override;
		bool getIllusion(Board& board, Player& player);
		void setSelectedMove(const Coord& coord, CombatCardType cardType);

	private:
		bool m_hasUserSelected;
		Coord m_selectedCoord;
		CombatCardType m_selectedCardType;
	};

	class Storm :public PowerCard {
	public:
		Storm();

		void apply(Board& board, Player& player) override;

	};

	class Tide :public PowerCard {
	public:
		Tide();

		void apply(Board& board, Player& player) override;
		std::vector<Coord>getStacks(const Board& board);
		const std::vector<Coord>& getAvailableChoices() const; 
		bool hasUserSelected() const;
		void setSelectedCoords(const Coord& from, const Coord& to); 

	private:
		std::vector<Coord> m_availableStacks;
		Coord m_selectedFrom;
		Coord m_selectedTo;
		bool m_hasUserSelected;
	};



	class Mist :public PowerCard {
	public:
		Mist();

		void apply(Board& board, Player& player) override;
		bool hasIllusion(Board& board, IllusionService& illusionService, Player& player);

	};

	class Wave :public PowerCard {
	public:
		Wave();

		void apply(Board& board, Player& player) override;
		std::vector<Coord>validStacks(const Board& board) const;
		const std::vector<Coord>& getAvailableStacks() const;
		const std::vector<Coord>& getAvailableSpaces() const;
		bool hasUserSelected() const;
		bool hasSelectedCard() const;

		void setSelectedMove(const Coord& from, const Coord& to);
		void setSelectedCard(char cardType);
		void clearSelections();
	private:
		mutable std::vector<Coord> m_availableStacks;
		std::vector<Coord> m_availableSpaces;
		Coord m_selectedFrom;
		Coord m_selectedTo;
		char m_selectedCardType;
		bool m_hasUserSelected;
		bool m_hasSelectedCard;

	};

	class Whirlpool :public PowerCard {
	

	public:
		Whirlpool();

		void setSelectedPair(const Coord& first, const Coord& second);
		void setTopCardPosition(const Coord& coord);
		const std::vector<std::pair<Coord, Coord>>& getAvailablePairs() const;
		std::vector<std::pair<Coord, Coord>> getPairs(Board& board);
		void apply(Board& board, Player& player) override;
	private:
		std::vector<std::pair<Coord, Coord>> m_availablePairs;
		std::pair<Coord, Coord> m_selectedPair;
		Coord m_topCardPosition;
		bool m_hasUserSelectedPair;
		bool m_hasUserSelectedTopCard;
	
	};

	class Blizzard :public PowerCard {
	public:
		enum class BlockType {
			NONE,
			ROW,
			COLUMN
		};

		Blizzard();
		void apply(Board& board, Player& player) override;

		const std::unordered_set<uint16_t>& getAvailableRows() const;
		const std::unordered_set<uint16_t>& getAvailableColumns() const;
		bool hasUserSelected() const;
		bool hasValidSelection() const;

		void setBlockType(BlockType type);
		void setSelectedValue(uint16_t value);
		void clearSelection();

	private:
		void updateAvailableChoices(const Board& board);

	private:
		std::unordered_set<uint16_t> m_availableRows;
		std::unordered_set<uint16_t> m_availableColumns;
		BlockType m_selectedBlockType;
		uint16_t m_selectedValue;
		bool m_hasUserSelected;


	};

	class Waterfall :public PowerCard {
	public:
		Waterfall();
		void apply(Board& board, Player& player) override;

		// Getters
		const std::unordered_map<Orientation, std::vector<int16_t>>& getAvailableOptions() const;
		bool hasUserSelected() const;

		// Setters
		void setSelection(Orientation orientation, int16_t index, MoveDirection direction);
		void clearSelection();

	private:
		std::unordered_map<Orientation, std::vector<int16_t>> getOptions(Board& board);
		bool validateSelection() const;

	private:
		std::unordered_map<Orientation, std::vector<int16_t>> m_availableOptions;
		Orientation m_selectedOrientation;
		int16_t m_selectedIndex;
		MoveDirection m_selectedDirection;
		bool m_hasUserSelected;
	};



	class Support :public PowerCard {
	public:
		Support();
		void apply(Board& board, Player& player) override;

		const std::vector<Coord>& getAvailableCards() const;
		bool hasUserSelected() const;

		void setSelectedCoord(const Coord& coord);
		void clearSelection();

	private:
		std::vector<Coord> CoordCardType(Board& board, const Player& player) const;
		CombatCardType getNextCardType(CombatCardType currentType) const;

	private:
		std::vector<Coord> m_availableCards;
		Coord m_selectedCoord;
		bool m_hasUserSelected;
	};

	class Earthquake :public PowerCard {
	public:

		Earthquake();
		void apply(Board& board, Player& player) override;


		const std::vector<Coord>& getAvailableTargets() const;
		bool hasValidTargets() const;

	private:
		void findValidTargets(const Board& board);

	private:
		std::vector<Coord> m_availableTargets;


	};

	class Crumble :public PowerCard {
	public:
		Crumble();
		void apply(Board& board, Player& player) override;

		const std::vector<Coord>& getAvailableCards() const;
		bool hasUserSelected() const;

		void setSelectedCoord(const Coord& coord);
		void clearSelection();

	private:
		std::vector<Coord> CoordCardType(Board& board, const Player& player) const;
		CombatCardType getPreviousCardType(CombatCardType currentType) const;

	private:
		std::vector<Coord> m_availableCards;
		Coord m_selectedCoord;
		bool m_hasUserSelected;
	
	};

	class Border :public PowerCard {
	public:
		Border();

		void apply(Board& board, Player& player) override;

	};
	

	class Avalanche :public PowerCard {
	public:
		Avalanche();
		void apply(Board& board, Player& player) override;

		// Getters
		const std::vector<std::pair<MoveDirection, std::pair<Coord, Coord>>>& getAvailableChoices() const;
		bool hasUserSelected() const;

		// Setters
		void setSelectedMove(MoveDirection direction, const Coord& firstCoord, const Coord& secondCoord);
		void clearSelection();

	private:
		std::vector<std::pair<Orientation, std::pair<Coord, Coord>>> getPairs(Board& board);
		std::vector<std::pair<MoveDirection, std::pair<Coord, Coord>>> checkShifting(
			const std::vector<std::pair<Orientation, std::pair<Coord, Coord>>>& pack,
			Board& board);
		bool validateAndApplyMove(Board& board, const MoveDirection& type,
			const std::pair<Coord, Coord>& selectedPair);

	private:
		std::vector<std::pair<MoveDirection, std::pair<Coord, Coord>>> m_availableChoices;
		MoveDirection m_selectedDirection;
		std::pair<Coord, Coord> m_selectedPair;
		bool m_hasUserSelected;

		const std::unordered_map<MoveDirection, Coord> directionDeltas = {
			{MoveDirection::Down, {0, 1}},
			{MoveDirection::Up, {0, -1}},
			{MoveDirection::Right, {2, 0}},
			{MoveDirection::Left, {-2, 0}}
		};
	};
	

	class Rock : public PowerCard {
	public:
		Rock();
		void apply(Board& board, Player& player) override;

		// Getters
		const std::vector<Coord>& getAvailableIllusions() const;
		bool hasUserSelected() const;
		bool hasCardSelected() const;

		// Setters
		void setSelectedCoord(const Coord& coord);
		void setSelectedCard(char cardType);
		void clearSelection();

	private:
		std::vector<Coord> getIllusionCoords(const Board& board) const;

	private:
		std::vector<Coord> m_availableIllusions;
		Coord m_selectedCoord;
		char m_selectedCardType;
		bool m_hasUserSelected;
		bool m_hasCardSelected;
	};

}