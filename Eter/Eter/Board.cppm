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
		Board();
		Board(uint16_t);

		void appendMove(Coord, CardPtr&&);
		void renderBoard() const;

	private:
		void _updateBoardBoundry(Coord);
		void _updateAvailableSpaces(Coord);
	
	private:
		struct CoordFunctor {
			size_t operator()(const Coord&) const;
		};

	private:
		uint16_t m_size;
		int16_t m_row_min, m_row_max;
		int16_t m_col_min, m_col_max;

		std::unordered_map<Coord, std::vector<CardPtr>, CoordFunctor> m_combat_cards;
		std::unordered_set<Coord, CoordFunctor> m_available_spaces;

	};
}