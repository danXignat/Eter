#include "ExplosionService.h"

#include "conio.h"

namespace base {
	ExplosionService::ExplosionService(Board& board)
		: m_board{board},
		m_card{board.size()} {
		
	}

	void ExplosionService::apply() {

	}

	void ExplosionService::setting() {
		bool is_setting = true;
		while (is_setting) {
			if (_kbhit()) {
				char ch = _getch();

				if (ch == -32) {
					ch = _getch();

					switch (ch) {
					case 77:							//right arrow
						m_card.rotateRight();
						m_card.render();
						break;

					case 75:							//left arrow
						m_card.rotateLeft();	
						m_card.render();
						break;

					default:
						break;
					}
				}
				else if (ch == 13) {					//enter
					is_setting = false;
				}
			}
		}
	}

	void ExplosionService::render_explosion() {
		m_card.render();
	}
}