#pragma once
#include <unordered_map>
#include <cstdint>

#include "Board.h"

#include "typedefs.h"

namespace base {
	class WinManager {
	public:

		WinManager(Board&);
		bool won() const;
		void addCard(const Coord&);
		void removeCard(const Coord&);

	private:
		void _setDiags();
		void _trySetWin(const Coord& coord);
		int16_t _getIncrement(const Coord&);

	public:
		static constexpr const int16_t blue_increment = 1;
		static constexpr const int16_t red_increment = -1;

	public:
		Board& board;
		uint16_t board_size;

		std::unordered_map<uint16_t, int16_t> m_rows, m_cols;
		int16_t m_diag1, m_diag2;

		bool m_are_diags_init;
		bool m_won;
	};
}