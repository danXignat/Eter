#pragma once
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <memory>
#include <algorithm>

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
		static constexpr const std::pair<uint16_t, uint16_t> START_POS{10, 5};

		Board(uint16_t);
		Board(GameSizeType);
		Board(const Board&) = delete;
		Board& operator=(const Board&) = delete;
		Board(Board&&) noexcept;
		Board& operator=(Board&&) noexcept;
		uint16_t size() const;

		void render() const;
		void sideViewRender();

		bool fixedWidth() const;
		bool fixedHeight() const;
		bool hasStack(const Coord&) const;
		bool isFixed() const;
		bool isCardOfColorAt(color::ColorType, const Coord&) const;
		uint16_t getSize() const;
		std::optional<CombatCardRef> getTopCard(Coord pos);
		Coord getLeftCorner() const;
		std::pair<Coord, Coord> getBoudingRect() const;
		std::pair<uint16_t, uint16_t> getBoundingRectSize() const;


		const std::vector<CombatCard>& operator[](const Coord& coord);

		void appendMove(const Coord&, CombatCard&&);
		void removeTopCardAt(const Coord& coord);
		void removeCardFromStackAt(const Coord& coord, const CombatCard& card_to_remove);
		void removeStack(const Coord& coord);
		void removeRow(uint16_t x);
		void removeColumn(uint16_t y);
		void moveRow(uint16_t from_y, uint16_t to_y);
		void moveColumn(uint16_t from_x, uint16_t to_x);
		bool moveStack(const Coord& from_coord, const Coord& to_coord);
		void swapStacks(const Coord& from_coord, const Coord& to_coord);

		bool isValidAddMove(const Coord&, const CombatCard&);
		bool isValidRemoveMove(std::vector<Coord>);
		bool isValidMovingMove(std::vector<Coord>);

		std::vector<Coord> availableSpaces();
		
		
	
	private:
		void _updateAvailableSpaces(Coord);
		void _reinitialise();

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

		std::unordered_map<Coord, std::vector<CombatCard>, CoordFunctor> m_combat_cards;
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