export module Board;

import <memory>;
import <vector>;
import <stack>;
import <unordered_map>;
import <unordered_set>;
import <vector>;

import CombatCard;

namespace base {
	using Coord = std::pair<int16_t, int16_t>;
	using CardPtr = std::unique_ptr<CombatCard>;

	export class Board {
	public:
		Board(uint16_t);

		void appendMove(Coord, CardPtr&&);
		void renderBoard() const;
		uint16_t getSize() const;
		bool isFixed() const;

	private:
		void _updateAvailableSpaces(Coord);
		bool _isValidPos(Coord) const;
	
	private:
		struct CoordFunctor {
			size_t operator()(const Coord&) const;
		};

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

		std::unordered_map<Coord, std::vector<CardPtr>, CoordFunctor> m_combat_cards;
		std::unordered_set<Coord, CoordFunctor> m_available_spaces;

	};
}