#pragma once
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <memory>
#include <algorithm>
#include <array>

#include "CombatCard.h"
#include "CombatCardType.h"
#include "Player.h"
#include "utils.h"
#include "colors.h"
#include "GameModeType.h"
#include "typedefs.h"

using namespace utils;

namespace base {
	class Board {
	public:
		using Stack = std::vector<CombatCard>;
		static constexpr const std::pair<uint16_t, uint16_t> START_POS{10, 5};

		Board(uint16_t, Player&, Player&);
		Board(GameSizeType, Player&, Player&);
		Board(const Board&) = delete;
		Board& operator=(const Board&) = delete;
		uint16_t size() const;

		bool fixedWidth() const;
		bool fixedHeight() const;
		bool hasStack(const Coord&) const;
		bool isFixed() const;
		bool isCardOfColorAt(color::ColorType, const Coord&) const;
		bool isFixedColumn(uint16_t x)const;
		bool isFixedRow(uint16_t y)const;
		
		uint16_t getSize() const;
		Coord getLeftCorner() const;
		std::pair<Coord, Coord> getBoudingRect() const;
		std::pair<uint16_t, uint16_t> getBoundingRectSize() const;
		const std::unordered_set<Coord, CoordFunctor>& availableSpaces() const;
		std::unordered_map<Coord, Stack, CoordFunctor>& getCombatCards();
		std::optional<CombatCardRef> getTopCard(const Coord& coord);
		const std::vector<CombatCard>& operator[](const Coord& coord);
		std::unordered_set<uint16_t> getFixedRows() const;
		std::unordered_set<uint16_t> getFixedColumns() const; 
		Coord getRightmostOnRow(uint16_t y) const;
		Coord getLeftmostOnRow(uint16_t y) const;
		Coord getTopmostOnColumn(uint16_t x) const;
		Coord getBottommostOnColumn(uint16_t x) const;
		
		void appendMove(const Coord&, CombatCard&&);
		void moveRow(uint16_t from_y, uint16_t to_y);
		void moveColumn(uint16_t from_x, uint16_t to_x);
		void moveStack(const Coord& from_coord, const Coord& to_coord);
		void swapStacks(const Coord& from_coord, const Coord& to_coord);
		void moveStacks(const std::vector<std::pair<Coord, Coord>>& moves);

		CombatCard&& popTopCardAt(const Coord& coord);
		CombatCard&& popCardFromStackAt(const Coord& coord, const CombatCard& card_to_remove);
		Stack&& popStack(const Coord& coord);
		std::vector<Stack> popRow(uint16_t y);
		std::vector<Stack> popColumn(uint16_t x);

		void removeTopCardAt(const Coord& coord);
		void removeCardFromStackAt(const Coord& coord, const CombatCard& card_to_remove);
		void removeStack(const Coord& coord);
		void removeRow(uint16_t y);
		void removeColumn(uint16_t x);

		bool isValidPlaceCard(const Coord&, const CombatCard&);
		bool isValidRemoveStack(const Coord&) const;
		bool isValidMoveStack(const Coord&, const Coord&) const;
		bool isValidMoveStacks(const std::vector<std::pair<Coord, Coord>>& moves) const;
		
		void render() const;
		void sideViewRender();

	private:
		void _updateAvailableSpaces(const Coord&);
		void _reinitialise();
		bool _isConex(const std::unordered_set<Coord, CoordFunctor>&) const;
		void _addUsedCard(CombatCard&&);
		void _addUsedStack(Stack&& stack);

	private:
		struct BoundingRect {
			uint16_t size;
			Coord corner1, corner2;
			bool fixed_width, fixed_height;

			BoundingRect();
			BoundingRect(uint16_t);
			BoundingRect(uint16_t size, const Coord& coord);

			void add(const Coord&);
			void remove(const Coord&);
			bool withinWidth(const Coord&) const;
			bool withinHeight(const Coord&) const;
			bool within(const Coord&) const;
			bool isFixed() const;
		};

	private:
		uint16_t m_size;
		BoundingRect m_bounding_rect;
		Player& m_player1, & m_player2;

		std::array<Coord, 8> m_adjacency_offsets;
		std::unordered_map<Coord, Stack, CoordFunctor> m_combat_cards;
		std::unordered_set<Coord, CoordFunctor> m_available_spaces;

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