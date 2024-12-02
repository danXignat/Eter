#include "WinManager.h"

namespace base {
	//---------------------------------Constructor------------------------------
	WinManager::WinManager(Board& board)
		: board{ board },
		board_size{ board.size() },
		m_diag1{ 0 },
		m_diag2{ 0 },
		m_are_diags_init{ false },
		m_won{ false }
	{}

	//---------------------------------Modifiers---------------------------------

	void WinManager::addCard(const Coord& coord) {
		const auto& [x, y] = coord;
		int16_t increment = _getIncrement(coord);

		m_rows[y] += increment;
		m_cols[x] += increment;

		if (board.isFixed() && m_are_diags_init == false) {
			_setDiags();
		}
		else if (m_are_diags_init == true) {
			const auto& [corner_x, corner_y] = board.getLeftCorner();

			uint16_t local_x = (x - corner_x) / 2;
			uint16_t local_y = y - corner_y;

			if (local_x == local_y) {
				m_diag1 += increment;
			}

			if (local_x + local_y == board_size - 1) {
				m_diag2 += increment;
			}
		}

		_trySetWin(coord);
	}

	void WinManager::removeCard(const Coord& coord) {

	}

	//----------------------------------------Getters--------------------------

	bool WinManager::won() const {
		return m_won;
	}

	//------------------------------------------Helpers------------------------------

	void WinManager::_setDiags() {
		for (uint16_t i = 0; i < board_size; i++) {
			const auto& [x, y] = board.getLeftCorner();

			Coord pos1{ x + 2 * i, y + i };
			Coord pos2{ x + 2 * (board_size - 1 - i), y + i };

			m_diag1 += _getIncrement(pos1);
			m_diag2 += _getIncrement(pos2);
		}

		m_are_diags_init = true;
	}

	void WinManager::_trySetWin(const Coord& coord) {
		const auto& [x, y] = coord;

		bool won_on_lines = abs(m_diag1) == board_size || abs(m_diag2) == board_size;
		bool won_on_diags = abs(m_rows[y]) == board_size || abs(m_cols[x]) == board_size;

		if (won_on_lines || won_on_diags) {
			m_won = true;
		}
	}

	int16_t WinManager::_getIncrement(const Coord& coord) {
		int16_t increment{0};

		if (auto card = board.getTopCard(coord)) {
			increment = (card->get().getColor() == color::ColorType::BLUE) ? 1 : -1;
		}

		return increment;
	}
}