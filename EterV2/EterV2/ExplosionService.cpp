#include "ExplosionService.h"

#include <ranges>
#include "conio.h"

namespace base {
	ExplosionService::ExplosionService(Board& board, Player& player1, Player& player2)
		: m_board{board},
		m_player1{ player1 }, m_player2{ player2 },
		card{board, player1, player2} {
		
	}

	bool ExplosionService::checkAvailability() const {
		std::unordered_map<uint16_t, uint16_t> x_field_counter;
		std::unordered_map<uint16_t, uint16_t> y_field_counter;

		for (const Coord& coord : m_board | std::views::keys) {
			x_field_counter[coord.first]++;
			y_field_counter[coord.second]++;
		}

		auto validCount = [&](const Coord& pair) { return pair.second == m_board.size(); };
		uint16_t valid_x_count = std::count_if(x_field_counter.begin(), x_field_counter.end(), validCount);
		uint16_t valid_y_count = std::count_if(y_field_counter.begin(), y_field_counter.end(), validCount);

		return valid_x_count + valid_y_count >= VALID_COMPLETED_LINES;
	}

	void ExplosionService::apply() {
		card.apply();
	}

	std::unordered_map<Coord, Effect, utils::CoordFunctor> ExplosionService::getEffectCoords() const {
		return card.getEffectCoords();
	}

	void ExplosionService::setting() {
		if (m_board.fixedWidth() == false) {
			card.setEffectForWidth(m_board);
		}
		else if (m_board.fixedHeight() == false) {
			card.setEffectForHeight(m_board);
		}

		card.color = true;
		card.render();
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
						card.rotateRight();
						card.render();
						break;

					case 75:							//left arrow
						card.rotateLeft();
						card.render();
						break;

					default:
						break;
					}

					break;
				}
				case 'a': {
					card.moveEffect(Direction::LEFT);
					card.render();
					break;
				}
				case 'd': {
					card.moveEffect(Direction::RIGHT);
					card.render();
					break;
				}
				case 's': {
					card.moveEffect(Direction::DOWN);
					card.render();
					break;
				}
				case 'w': {
					card.moveEffect(Direction::UP);
					card.render();
					break;
				}
				case 13:
					is_setting = false;
					break;
				}
			}
		}
	}

	void ExplosionService::renderExplosion() {
		card.render();
	}

}