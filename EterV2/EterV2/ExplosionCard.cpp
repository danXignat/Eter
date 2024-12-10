#include "ExplosionCard.h"

#include "logger.h"
#include "utils.h"

using namespace logger;

namespace base {
	Explosion::Explosion(uint16_t size) :
		m_board_size{ size },
		m_effects{ size, std::vector<std::optional<Effect>>(size) },
		m_effect_corner1{ 0, 0 }, m_effect_corner2{ size - 1, size - 1 } {

		std::random_device rd;
		std::mt19937 gen(rd());

		m_effect_count = _generateEffectCount(gen);
		std::vector<Coord> positions{ _generateEffectPos(gen) };
		std::vector<Effect> effects{ _generateEffectType(gen) };

		for (uint16_t i = 0; i < m_effect_count; ++i) {
			auto [row, col] = positions[i];
			m_effects[row][col] = effects[i];

			Logger::log(
				Level::INFO, "explosion effect [{}, {}] -> {}",
				row, col,
				static_cast<int>(effects[i])
			);
		}

	}

	void Explosion::apply() {
		for (uint16_t i = m_effect_corner1.second; i <= m_effect_corner2.second; i++) {
			for (uint16_t j = m_effect_corner1.first; j <= m_effect_corner2.first; j++) {

			}
		}
	}

	void Explosion::setEffectForWidth(Board& board) {
		auto br_size = board.getBoundingRectSize();

		m_effect_corner2.first = br_size.first - 1;
	}

	void Explosion::setEffectForHeight(Board& board) {
		auto br_size = board.getBoundingRectSize();

		m_effect_corner2.second = br_size.second - 1;
	}

	void Explosion::moveEffect(Direction dir) {
		switch (dir) {
		case base::Direction::RIGHT: {
			if (m_effect_corner2.first + 1 < m_board_size) {
				m_effect_corner1.first++;
				m_effect_corner2.first++;
			}
			break;
		}

		case base::Direction::LEFT: {
			if (m_effect_corner1.first - 1 >= 0) {
				m_effect_corner1.first--;
				m_effect_corner2.first--;
			}
			break;
		}

		case base::Direction::UP: {
			if (m_effect_corner1.second - 1 >= 0) {
				m_effect_corner1.second--;
				m_effect_corner2.second--;
			}
			break;
		}

		case base::Direction::DOWN: {
			if (m_effect_corner2.second + 1 < m_board_size) {
				m_effect_corner1.second++;
				m_effect_corner2.second++;
			}
			break;
		}

		default:
			break;
		}
	}

	uint16_t Explosion::_generateEffectCount(std::mt19937& gen) {
		uint16_t begin = (m_board_size == 3) ? MIN_EFFECTS_3x3 : MIN_EFFECTS_4x4;
		uint16_t end = (m_board_size == 3) ? MAX_EFFECTS_3x3 : MAX_EFFECTS_4x4;

		std::uniform_int_distribution<uint16_t> random{ begin, end };
		return random(gen);
	}

	std::vector<Coord> Explosion::_generateEffectPos(std::mt19937& gen) {
		std::vector<Coord> positions;
		positions.reserve(m_board_size * m_board_size);

		for (uint16_t i = 0; i < m_board_size; ++i) {
			for (uint16_t j = 0; j < m_board_size; ++j) {
				positions.push_back({ i, j });
			}
		}

		std::shuffle(positions.begin(), positions.end(), gen);

		return { positions.begin(), positions.begin() + m_effect_count };
	}

	std::vector<Effect> Explosion::_generateEffectType(std::mt19937& gen) {
		std::uniform_int_distribution<uint16_t> dist{ 0, 9 };
		std::vector<Effect> effects;
		effects.reserve(m_effect_count);

		for (uint16_t i = 0; i < m_effect_count; i++) {
			uint16_t random_num = dist(gen);

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

	void Explosion::render() {
		Coord print_point{ 26, 14 };

		utils::printAtCoordinate("Explosion:", print_point.first, print_point.second);

		std::unordered_map<Effect, char> effect_map = {
			{Effect::HAND, 'B'},
			{Effect::REMOVE, 'R'},
			{Effect::HOLE, 'H'}
		};

		for (int16_t i = 0; i < m_board_size; ++i) {
			for (int16_t j = 0; j < m_board_size; ++j) {
				if (m_effects[i][j]) {
					std::cout << ((
						color == true && 
						m_effect_corner1.second <= i && i <= m_effect_corner2.second &&
						m_effect_corner1.first <= j && j <= m_effect_corner2.first
						) ? 
						to_string(color::ColorType::RED) :
						to_string(color::ColorType::DEFAULT));

					utils::printAtCoordinate(
						std::format("{}{}",
							effect_map[m_effects[i][j].value()], ' '),
						print_point.first + 2 * j, print_point.second + i + 1
					);

					std::cout << to_string(color::ColorType::DEFAULT);
				}
				else {
					std::cout << ((
						color == true &&
						m_effect_corner1.second <= i && i <= m_effect_corner2.second &&
						m_effect_corner1.first <= j && j <= m_effect_corner2.first
						) ?
						to_string(color::ColorType::RED) :
						to_string(color::ColorType::DEFAULT));

					utils::printAtCoordinate(
						"- ",
						print_point.first + 2 * j, print_point.second + i + 1
					);

					std::cout << to_string(color::ColorType::DEFAULT);
				}
			}
		}
	}

	void Explosion::rotateLeft() {
		uint16_t size = m_board_size;
		auto& mat = m_effects;

		for (uint16_t i = 0; i < size / 2; ++i) {
			for (uint16_t j = i; j < size - i - 1; ++j) {
				auto temp = mat[i][j];
				mat[i][j] = mat[j][size - i - 1];
				mat[j][size - i - 1] = mat[size - i - 1][size - j - 1];
				mat[size - i - 1][size - j - 1] = mat[size - j - 1][i];
				mat[size - j - 1][i] = temp;
			}
		}
	}

	void Explosion::rotateRight() {
		uint16_t size = m_board_size;
		auto& mat = m_effects;

		for (uint16_t i = 0; i < size / 2; ++i) {
			for (uint16_t j = i; j < size - i - 1; ++j) {
				auto temp = mat[i][j];
				mat[i][j] = mat[size - j - 1][i];//mat[j][size - i - 1]
				mat[size - j - 1][i] = mat[size - i - 1][size - j - 1];
				mat[size - i - 1][size - j - 1] = mat[j][size - i - 1];
				mat[j][size - i - 1] = temp;
			}
		}
	}

}