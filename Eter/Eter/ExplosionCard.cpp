#include <ranges>
module ExplosionCard;

namespace base {

	Explosion::Explosion(uint16_t size):
		m_board_size{ size },
		m_generator{ m_random() },
		m_effect_count{ _generateEffectCount() } {

		m_effects.reserve(m_effect_count);

		auto positions = _generateEffectPos();
		auto effects = _generateEffectType();

		for (uint16_t i = 0; i < positions.size(); i++) {
			m_effects.emplace(positions[i], effects[i]);
		}
	}

	uint16_t Explosion::_generateEffectCount() {
		uint16_t begin = (m_board_size == 3) ? 2 : 3;
		uint16_t end = (m_board_size == 3) ? 4 : 6;

		std::uniform_int_distribution<uint16_t> random{ begin, end };
		return random(m_generator);
	}

	std::vector<Coord>& Explosion::_generateEffectPos() {
		std::vector<Coord> positions;
		positions.reserve(m_board_size * m_board_size);

		for (uint16_t i = 0; i < m_board_size; i++) {
			for (uint16_t j = 0; j < m_board_size; j++) {
				positions.emplace_back(i, j);
			}
		}

		std::shuffle(positions.begin(), positions.end(), m_generator);

		std::vector<Coord> res{ positions.begin(), positions.begin() + m_board_size };

		return res;
	}

	std::vector<Effect>& Explosion::_generateEffectType() {
		std::uniform_int_distribution<uint16_t> dist{ 0, 9 };
		std::vector<Effect> effects;
		effects.reserve(m_effect_count);

		for (uint16_t i = 0; i < m_effect_count; i++) {
			uint16_t random_num = dist(m_generator);

			if (random_num == 0) {
				effects.push_back(Effect::HOLE);
			}
			else {
				effects.push_back(
					(random_num % 2) ? Effect::HAND : Effect::REMOVE
				);
			}
		}

		return effects;
	}

}