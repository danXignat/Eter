#pragma once
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <memory>

#include "CombatCard.h"
#include "CombatCardType.h"
#include "Player.h"
#include "utils.h"
#include "colors.h"
#include "typedefs.h"

using namespace utils;

namespace base {
	
	class Board {
	public:
		Board(uint16_t);

		void appendMove(const Coord&, CombatCard&&);
		void renderBoard() const;
		uint16_t getSize() const;
		bool isFixed() const;
		bool isValidMove(const Coord&, const CombatCard&);
		uint16_t size() const;
		std::optional<std::reference_wrapper<CombatCard>> getTopCard(Coord pos);
		Coord getLeftCorner() const;


		void removeTopCardAt(const Coord& coord);
		void removeRow(const Coord& start_coord, const Player& player);
		void removeColumn(const Coord& start_coord, const Player& player);
		void moveStack(const Coord& fromCoord, const Coord& toCoord, Player& player, bool isOpponent);

	private:
		void _updateAvailableSpaces(Coord);

	private:
		struct BoundingRect {
			uint16_t size;
			Coord corner1, corner2;
			bool fixed_width, fixed_height;

			BoundingRect(uint16_t);
			void add(Coord);
			void remove(Coord);
			bool withinWidth(Coord) const;
			bool withinHeight(Coord) const;
			bool within(Coord) const;
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

		using iterator = MapType::iterator;
		using const_iterator = MapType::const_iterator;

		iterator begin();
		iterator end();

		const_iterator begin() const;
		const_iterator end() const;
	};	
}