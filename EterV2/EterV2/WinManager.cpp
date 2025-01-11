#include "WinManager.h"

namespace base {
	//---------------------------------Constructor------------------------------
	WinManager::WinManager(uint16_t size)
		: board_size{ size },
		m_diag1{ 0 },
		m_diag2{ 0 },
		m_is_fixed{ false },
		m_won{ false }
	{}

	//---------------------------------Modifiers---------------------------------

	void WinManager::addCard(const Coord& coord) {
		const auto& [x, y] = coord;
		int16_t increment = _getIncrement(coord);

		m_rows[y] += increment;
		m_cols[x] += increment;

		if (m_is_fixed) {

		}

		_trySetWin(coord);
	}

	void WinManager::removeCard(const Coord& coord) {

	}

	void WinManager::setFixed(bool fixed) {
		if (m_is_fixed == true && fixed == false) {
			m_diag1 = 0;
			m_diag2 = 0;
		}

		m_is_fixed = fixed;
	}

	//----------------------------------------Getters--------------------------

	bool WinManager::won() const {
		return m_won;
	}

	//------------------------------------------Helpers------------------------------

	void WinManager::_setDiags() {
		/*for (uint16_t i = 0; i < board_size; i++) {
			const auto& [x, y] = board.getLeftCorner();

			Coord pos1{ x + 2 * i, y + i };
			Coord pos2{ x + 2 * (board_size - 1 - i), y + i };

			auto getIncrement = [&](const Coord& coord) {
				if (board.getTopCard(coord)->get().getColor() == color::ColorType::BLUE)
					return blue_increment;
				else
					return red_increment;
				};

			m_diag1 += board.hasStack(pos1) ? getIncrement(pos1) : 0;
			m_diag2 += board.hasStack(pos2) ? getIncrement(pos2) : 0;
		}

		m_are_diags_init = true;*/
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
		/*using namespace color;

		int16_t increment{0};
		size_t stack_size = board[coord].size();
		ColorType color_last = board[coord][stack_size - 1].getColor();

		if (stack_size == 1) {
			increment = (color_last == color::ColorType::BLUE) ? blue_increment : red_increment;
		}
		else if (stack_size > 1){
			ColorType color_last_but_one = board[coord][stack_size - 2].getColor();

			if (color_last == ColorType::RED && color_last_but_one == ColorType::BLUE) {
				increment = 2 * red_increment;
			}
			else if (color_last == ColorType::BLUE && color_last_but_one == ColorType::RED) {
				increment = 2 * blue_increment;
			}
		}

		return increment;*/
		return 1;
	}
}