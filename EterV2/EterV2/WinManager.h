#pragma once
#include <unordered_map>
#include <cstdint>

#include "typedefs.h"

namespace base {
	class WinManager {
	public:

		WinManager(uint16_t);
		bool won() const;
		void addCard(const Coord&);
		void removeCard(const Coord&);
		void setFixed(bool);

	private:
		void _setDiags();
		void _trySetWin(const Coord& coord);
		int16_t _getIncrement(const Coord&);

	public:
		static constexpr const int16_t blue_increment = 1;
		static constexpr const int16_t red_increment = -1;

	public:
		uint16_t board_size;

		std::unordered_map<uint16_t, int16_t> m_rows, m_cols;
		int16_t m_diag1, m_diag2;

		bool m_is_fixed;
		bool m_won;
	};
}