#include "ExplosionService.h"

#include "conio.h"

namespace base {
	ExplosionService::ExplosionService(Board& board, Player& player1, Player& player2)
		: m_board{board},
		m_player1{ player1 }, m_player2{ player2 },
		m_card{board.size()} {
		
	}

	void ExplosionService::apply() {
		
	}

	void ExplosionService::setting() {
		if (m_board.fixedWidth() == false) {
			m_card.setEffectForWidth(m_board);
		}
		else if (m_board.fixedHeight() == false) {
			m_card.setEffectForHeight(m_board);
		}

		m_card.color = true;
		m_card.render();
		bool is_setting = true;
		while (is_setting) {
			if (_kbhit()) {
				char ch = _getch();
				switch (ch)
				{

				case -32: {
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

					break;
				}
				case 'a': {
					m_card.moveEffect(Direction::LEFT);
					m_card.render();
					break;
				}
				case 'd': {
					m_card.moveEffect(Direction::RIGHT);
					m_card.render();
					break;
				}
				case 's': {
					m_card.moveEffect(Direction::DOWN);
					m_card.render();
					break;
				}
				case 'w': {
					m_card.moveEffect(Direction::UP);
					m_card.render();
					break;
				}
				case 13:
					is_setting = false;
					break;
				}
			}
		}
	}

	void ExplosionService::render_explosion() {
		m_card.render();
	}
}