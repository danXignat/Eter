#pragma once

#include "Board.h"
#include "Player.h"
#include "MageCard.h"
#include "typedefs.h"
#include "logger.h"

namespace base {
	class MasterOfFireFront : public MageCard {
	public:
		MasterOfFireFront();
		bool apply(Board& board, Player& player) override;
		std::vector<Coord> getChoices(const Board& board, const Player& player);
		void setSelectedCoord(const Coord& coord);
		const std::vector<Coord>& getAvailableChoices()const;
	private:
		std::vector<Coord>m_availableChoices;
		bool m_hasUserSelected = false;
		Coord m_selectedCoord;
	};

	class MasterOfFireBack : public MageCard {
	public:
		MasterOfFireBack();

		bool apply(Board& board, Player& player) override;

		std::pair<std::vector<uint16_t>, std::vector<uint16_t>> getChoices(const Board& board, const Player& player);
		const std::vector<uint16_t>& getAvailableRows()const;
		const std::vector<uint16_t>& getAvailableColumns()const;
		bool hasUserSelected()const;
		void setSelectedChoice(uint16_t choice, char type);
		void clearSelection();
	private:
		std::vector<uint16_t> m_availableRows;
		std::vector<uint16_t> m_availableColumns;
		bool m_hasUserSelected = false;
		uint16_t m_selectedChoice;
		char m_selectedType; // 'r' for row, 'c' for column
	};

	class MasterOfEarthFront : public MageCard {
	public:
		MasterOfEarthFront();

		bool apply(Board& board, Player& player) override;
		bool validPosition(const Coord& coord, Board& board, const Player& player)const;
		std::vector<Coord> getChoices(Board& board, const Player& player);
		void setSelectedCoord(const Coord& coord);
		void setSelectedCardType(CombatCardType cardType);

	private:
		std::vector<Coord>m_availableChoices;
		Coord m_selectedCoord;
		CombatCardType m_selectedCardType;

		bool m_hasUserSelected = false; // pt coordonata
		bool m_hasCardTypeSelected = false; // pt tip card 
	};

	class MasterOfEarthBack : public MageCard {
	public:
		MasterOfEarthBack();

		bool apply(Board& board, Player& player) override;
		void setSelectedCoord(const Coord& coord);
		const std::vector<Coord>& getAvailableChoices(const Board& board) const;
	private:
		mutable std::vector<Coord> m_availableChoices;
		Coord m_selectedCoord;
		bool m_hasUserSelected = false;
	};

	class MasterOfAirFront : public MageCard {
	public:
		MasterOfAirFront();

		bool apply(Board& board, Player& player) override;
		//std::vector<Coord> getChoices(Board& board, const Player& player);
		void setMoveFrom(const Coord& coord);
		void setMoveTo(const Coord& coord);
		const std::vector<Coord>& getAvailableChoices(const Board& board, const Player& player);
	private:
		Coord m_moveFrom;
		Coord m_moveTo;
		mutable std::vector<Coord> m_availableChoices;
		bool m_hasUserSelectedFrom = false;
		bool m_hasUserSelectedTo = false;
	};

	class MasterOfAirBack : public MageCard {
	public:
		MasterOfAirBack();

		bool apply(Board& board, Player& player) override;
		void setSelectedCoord(const Coord& coord);
		const std::vector<Coord>& getAvailableChoices(const Board& board) const;

	private:
		Coord m_selectedCoord;
		mutable std::vector<Coord> m_availableChoices;
		bool m_hasUserSelected = false;
	};

	class MasterOfWaterFront : public MageCard {
	public:
		MasterOfWaterFront();
		bool apply(Board& board, Player& player) override;
		void setMoveFrom(const Coord& coord);
		void setMoveTo(const Coord& coord);
		const std::vector<Coord>& getAvailableChoices(const Board& board, const Player& player);
	private:
		Coord m_moveFrom;
		Coord m_moveTo;
		mutable std::vector<Coord> m_availableChoices;
		bool m_hasUserSelectedFrom = false;
		bool m_hasUserSelectedTo = false;
	};

	class MasterOfWaterBack : public MageCard {
	public:
		MasterOfWaterBack();
		std::optional<std::unordered_map<BorderType, std::vector<Coord>>> getBorders(Board& board, Player& player) const;
		//std::pair<uint16_t,char> selectBorders(const std::optional<std::unordered_map<BorderType, std::vector<Coord>>>& borders,Board&board)const ;
		void setSelectedBorder(uint16_t index);
		void setSelectedDestination(int destination);
		bool apply(Board& board, Player& player) override;
	private:
		uint16_t m_selectedBorderIndex = 0;
		int m_selectedDestination = 0;
		bool m_hasSelectedBorder = false;
		bool m_hasSelectedDestination = false;
	};
	/*enum class BorderType {
		Top,
		Bottom,
		Left,
		Right
	};*/
}

