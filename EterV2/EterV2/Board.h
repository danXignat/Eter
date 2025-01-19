#pragma once
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <memory>
#include <algorithm>
#include <array>

#include "WinManager.h"
#include "CombatCard.h"
#include "CombatCardType.h"
#include "Player.h"
#include "utils.h"
#include "colors.h"
#include "GameModeType.h"
#include "typedefs.h"
#include "Config.h"

namespace base {
	class Board {
	public:
		using Stack = std::vector<CombatCard>;

		Board(GameSizeType, Player&, Player&);
		Board(const Board&) = delete;
		Board& operator=(const Board&) = delete;
		uint16_t size() const;

		bool fixedWidth() const;
		bool fixedHeight() const;
		bool hasStack(const Coord&) const;
		bool isFixed() const;
		bool isCardOfColorAt(color::ColorType, const Coord&) const;
		bool isFixedColumn(int16_t x) const;
		bool isFixedRow(int16_t y) const;
        
		uint16_t getSize() const;
		Coord getLeftCorner() const;
		std::pair<Coord, Coord> getBoudingRect() const;
		std::pair<uint16_t, uint16_t> getBoundingRectSize() const;
		const std::unordered_set<Coord, utils::CoordFunctor>& availableSpaces() const;
		const std::unordered_map<Coord, Stack, utils::CoordFunctor>& getCombatCards() const;
		std::optional<CombatCardRef> getTopCard(const Coord& coord);
		const std::vector<CombatCard>& operator[](const Coord& coord) const;
		std::unordered_set<int16_t> getFixedRows() const;
		std::unordered_set<int16_t> getFixedColumns() const; 
		Coord getRightMostOnRow(int16_t y) const;
		Coord getLeftMostOnRow(int16_t y) const;
		Coord getTopMostOnColumn(int16_t x) const;
		Coord getBottomMostOnColumn(int16_t x) const;
		std::vector<Coord> getCoordsOnRow(int16_t row_index) const;
		std::vector<Coord> getCoordsOnColumn(int16_t col_index) const;
		std::optional<Coord> getWinCoord() const;
	    
		void createHole(const Coord&);
		void appendMove(const Coord&, CombatCard&&);
		void moveRow(uint16_t from_y, uint16_t to_y);
		void moveColumn(uint16_t from_x, uint16_t to_x);
		void moveStack(const Coord& from_coord, const Coord& to_coord);
		void swapStacks(const Coord& from_coord, const Coord& to_coord);
		void moveStacks(const std::vector<std::pair<Coord, Coord>>& moves);
		void shiftRowToRight(const std::vector<Coord>& row_coords);
		void shiftRowToLeft(const std::vector<Coord>& row_coords);
		void shiftColumnDown(const std::vector<Coord>& col_coords);
		void shiftColumnUp(const std::vector<Coord>& col_coords);
		void mergeStacks(const std::vector<Coord>& coords, const Coord& final_coord);
		void appendSpecialCard(const Coord& coord, CombatCard&& card);
		void blockRow(uint16_t row, color::ColorType owner);
		void blockColumn(uint16_t column, color::ColorType owner);
		void clearBlockedRow();
		void clearBlockedColumn();

		CombatCard&& popTopCardAt(const Coord& coord);
		CombatCard&& popCardFromStackAt(const Coord& coord, const CombatCard& card_to_remove);
		Stack popStack(const Coord& coord);
		std::vector<Stack> popRow(uint16_t y);
		std::vector<Stack> popColumn(uint16_t x);

		void removeTopCardAt(const Coord& coord);
		void removeCardFromStackAt(const Coord& coord, const CombatCard& card_to_remove);
		void removeStack(const Coord& coord);
		void removeRow(uint16_t y);
		void removeColumn(uint16_t x);

		bool isValidPlaceCard(const Coord&, const CombatCard&);
		bool isValidRemoveStack(const Coord&) const;
		bool isValidRemoveRow(const uint16_t row_index)const;
		bool isValidRemoveColumn(const uint16_t col_index)const;
		bool isValidMoveStack(const Coord&, const Coord&) const;
		bool isValidMoveStacks(const std::vector<std::pair<Coord, Coord>>& moves) const;
		bool isValidMergeStacks(const std::vector<Coord>& coords, const Coord& final_coord)const;
		bool isRowBlocked(uint16_t row, color::ColorType player_color) const;
		bool isColumnBlocked(uint16_t column, color::ColorType player_color) const;

		void render() const;
		void sideViewRender();

	private:
		void _setWinPosition(const Coord&);
		void _updateAvailableSpaces(const Coord&);
		void _reinitialise();
		bool _isConex(const std::unordered_set<Coord, utils::CoordFunctor>&) const;
		void _addUsedCard(CombatCard&&);
		void _addUsedStack(Stack&& stack);

	private:
		struct BoundingRect {
			BoundingRect();
			BoundingRect(uint16_t);
			BoundingRect(uint16_t size, const Coord& coord);

			void add(const Coord&);
			void remove(const Coord&);
			bool withinWidth(const Coord&) const;
			bool withinHeight(const Coord&) const;
			bool within(const Coord&) const;
			bool isFixed() const;

			uint16_t size;
			Coord corner1, corner2;
			bool fixed_width, fixed_height;
		};

	private:
		Player& m_player1, & m_player2;

		std::optional<Coord> m_win_pos;
		uint16_t m_size;
		BoundingRect m_bounding_rect;

		std::unordered_map<Coord, Stack, utils::CoordFunctor> m_combat_cards;
		std::unordered_set<Coord, utils::CoordFunctor> m_available_spaces;
		std::optional<uint16_t> m_blocked_row;
		std::optional<uint16_t> m_blocked_column;
		color::ColorType m_blocked_row_owner;

	///------------------------------------------------Iterator---------------------------------------------

	public:
		using KeyType = Coord;
		using ValueType = std::vector<CombatCard>;
		using MapType = std::unordered_map<KeyType, ValueType>;

		using const_iterator = MapType::const_iterator;

		const_iterator begin() const;
		const_iterator end() const;
	};  
}