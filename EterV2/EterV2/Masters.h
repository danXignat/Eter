#pragma once

#include "Board.h"
#include "Player.h"
#include "MageCard.h"
#include "typedefs.h"
#include "logger.h"


namespace base {
	enum class LineChoice : uint16_t {
		None,
		Column,
		Row
	};

	enum class SideChoice : uint16_t {
		None,
		Top,
		Bottom,
		Left,
		Right
	};

	class MasterOfFireFront : public MageCard {
	public:
		MasterOfFireFront(Board& board, Player& player_red, Player& player_blue, color::ColorType color);
		bool apply() override;
		std::unordered_set<uint16_t> getChoices();
		void setSelectedCoord(const Coord& coord);
		const std::unordered_set<uint16_t>& getAvailableChoices()const;

	private: 
		std::unordered_set<uint16_t> m_availableChoices;
		std::optional<Coord> m_selectedCoord;  
	};

	class MasterOfFireBack : public MageCard {
	public:
		MasterOfFireBack(Board& board, Player& player_red, Player& player_blue, color::ColorType color);

		bool apply() override;

		std::unordered_set<uint16_t> getChoices();
		std::unordered_set<uint16_t> getRemoveChoices(const Coord& coord);

		const std::vector<uint16_t>& getAvailableRows()const;
		const std::vector<uint16_t>& getAvailableColumns()const;

		bool hasUserSelected()const;
		void setSelectedChoice(const Coord& coord);
		void clearSelection();

	private:
		void _setRowChoices();
		void _setColChoices();

	private:
		std::unordered_map<uint16_t, std::unordered_set<uint16_t>> m_availableRows;
		std::unordered_map<uint16_t, std::unordered_set<uint16_t>> m_availableColumns;

		bool m_hasUserSelected = false;
		uint16_t m_selectedChoice;
		LineChoice line_choice;
	};

	class MasterOfEarthFront : public MageCard {
	public:
		MasterOfEarthFront(Board& board, Player& player_red, Player& player_blue, color::ColorType color);

		bool apply() override;
		bool validPosition(const Coord& coord, CombatCardType other_type) const;
		std::unordered_set<uint16_t> getChoices(CombatCardType type);
		void setSelectedCoord(const Coord& coord);
		void setSelectedCard(uint16_t id);

	private:
		std::unordered_set<uint16_t>m_availableChoices;
		CombatCardType m_selectedCardType;
		Coord m_selectedCoord;
		uint16_t m_selectedId;

		bool m_hasUserSelected = false; // pt coordonata
		bool m_hasCardTypeSelected = false; // pt tip card 
	};

	class MasterOfEarthBack : public MageCard {
	public:
		MasterOfEarthBack(Board& board, Player& player_red, Player& player_blue, color::ColorType color);

		bool apply() override;
		void setSelectedCoord(const Coord& coord);
		const std::vector<Coord>& getAvailableChoices() const;
	private:
		mutable std::vector<Coord> m_availableChoices;
		Coord m_selectedCoord;
		bool m_hasUserSelected = false;
	};

	class MasterOfAirFront : public MageCard {
	public:
		MasterOfAirFront(Board& board, Player& player_red, Player& player_blue, color::ColorType color);

		bool apply() override;
		//std::vector<Coord> getChoices(Board& board, const Player& player);
		void setMoveFrom(const Coord& coord);
		void setMoveTo(const Coord& coord);
		const std::unordered_set<uint16_t>& getAvailableChoicesFrom();

		std::unordered_set<uint16_t> getIDSAt(const Coord& coord);

		const std::unordered_set<Coord, utils::CoordFunctor>   & getAvailableChoicesTo();

		Coord m_moveFrom;
		Coord m_moveTo;
		std::unordered_set<uint16_t> m_stackBufer;

	private:
		std::unordered_set<uint16_t> m_availableChoicesFrom;
		std::unordered_set<Coord, utils::CoordFunctor> m_availableChoicesTo;
		bool m_hasUserSelectedFrom = false;
		bool m_hasUserSelectedTo = false;
	};

	class MasterOfAirBack : public MageCard {
	public:
		MasterOfAirBack(Board& board, Player& player_red, Player& player_blue, color::ColorType color);

		bool apply() override;
		void setSelectedCoord(const Coord& coord);
		const std::vector<Coord>& getAvailableChoices(const Board& board) const;
		uint16_t createEter();

	private:
		std::optional<CombatCard> eter;
		Coord m_selectedCoord;
		mutable std::vector<Coord> m_availableChoices;
		bool m_hasUserSelected = false;
	};

	class MasterOfWaterFront : public MageCard {
	public:
		MasterOfWaterFront(Board& board, Player& player_red, Player& player_blue, color::ColorType color);
		bool apply() override;
		//std::vector<Coord> getChoices(Board& board, const Player& player);
		void setMoveFrom(const Coord& coord);
		void setMoveTo(const Coord& coord);
		const std::unordered_set<uint16_t>& getAvailableChoicesFrom();

		std::unordered_set<uint16_t> getIDSAt(const Coord& coord);

		const std::unordered_set<Coord, utils::CoordFunctor>& getAvailableChoicesTo();

		Coord m_moveFrom;
		Coord m_moveTo;
		std::unordered_set<uint16_t> m_stackBufer;

	private:
		std::unordered_set<uint16_t> m_availableChoicesFrom;
		std::unordered_set<Coord, utils::CoordFunctor> m_availableChoicesTo;
		bool m_hasUserSelectedFrom = false;
		bool m_hasUserSelectedTo = false;
	};

	class MasterOfWaterBack : public MageCard {
	public:
		MasterOfWaterBack(Board& board, Player& player_red, Player& player_blue, color::ColorType color);
		std::optional<std::unordered_map<BorderType, std::vector<Coord>>> getBorders(Board& board, Player& player) const;
		
		void setSelectedBorder(uint16_t index);
		void setSelectedDestination(int destination);
	    bool apply() override;

		std::unordered_set<uint16_t> getChoices();
		std::unordered_set<Coord, utils::CoordFunctor> getAvailableSpaces();
		void setStart(const Coord& coord);
		void setEnd(const Coord& coord);
		void resetBuffer();

	private:
		void _setRowChoices();
		void _setColChoices();

	public:
		std::unordered_map <Coord, std::vector <uint16_t>, utils::CoordFunctor> moving_cards_buffer;
		uint16_t moving_line;
		LineChoice line_choice;

	private:
		uint16_t m_selectedFrom = 0;
		uint16_t m_selectedTo = 0;

		bool m_setStart = false;
		bool m_setEnd = false;

		Coord m_end;
		std::unordered_map<uint16_t, std::unordered_set<uint16_t>> m_availableRows;
		std::unordered_map<uint16_t, std::unordered_set<uint16_t>> m_availableColumns;
		bool m_hasUserSelected = false;
		uint16_t m_selectedChoice;
	};
	/*enum class BorderType {
		Top,
		Bottom,
		Left,
		Right
	};*/
}

