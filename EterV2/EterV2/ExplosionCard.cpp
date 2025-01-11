#include "ExplosionCard.h"

#include <ranges>

#include "Config.h"
#include "logger.h"
#include "utils.h"

using namespace logger;

namespace base {
	Explosion::Explosion(Board& board, Player& player1, Player& player2) :
		m_board{board},
		m_player1{ player1 }, m_player2{ player2 },
		m_effects{ board.size(), std::vector<std::optional<Effect>>(board.size())},
		m_effect_corner1{ 0, 0 }, m_effect_corner2{ board.size() - 1, board.size() - 1 } {

		std::random_device rd;
		std::mt19937 gen(rd());

		m_effect_count = _generateEffectCount(gen);
		std::vector<Coord> positions{ _generateEffectPos(gen) };
		std::vector<Effect> effects{ _generateEffectType(gen) };

		/*std::vector<Coord> positions{ {0, 1}, {0, 2}, {0, 3} };
		std::vector<Effect> effects{ Effect::HAND, Effect::REMOVE, Effect::REMOVE };*/

		for (const auto&[coord, effect] : std::views::zip(positions, effects)) {
			auto& [row, col] = coord;
			m_effects[row][col] = effect;

			Logger::log(
				Level::INFO, "explosion effect [{}, {}] -> {}",
				row, col,
				static_cast<int>(effect)
			);
		}
	}

	void Explosion::apply() {
		m_board_corner.emplace(m_board.getBoudingRect().first);

		for (uint16_t i = m_effect_corner1.second; i <= m_effect_corner2.second; i++) {
			for (uint16_t j = m_effect_corner1.first; j <= m_effect_corner2.first; j++) {
				if (m_effects[i][j].has_value() == false || m_visited_effects.contains({j, i})) {
					continue;
				}
				
				Coord board_coord{ _mapExplosionToBoard({j, i}) };

				switch (m_effects[i][j].value()) {
				case Effect::HAND:
					_handleBackToHand({ j, i }, board_coord);
					break;

				case Effect::REMOVE:
					_handleRemove({ j, i }, board_coord);
					break;

				case Effect::HOLE:
					_handleHole({ j, i }, board_coord);
					break;

				default:
					break;
				}
			}
		}
	}

	std::unordered_map<Coord, Effect, utils::CoordFunctor> Explosion::getEffectCoords() const {
		std::unordered_map<Coord, Effect, utils::CoordFunctor> rez;
		rez.reserve(m_effect_count);

		for (int16_t i = 0; i < m_board.size(); ++i) {
			for (int16_t j = 0; j < m_board.size(); ++j) {
				rez[{i, j}] = *m_effects.at(i).at(j);
			}
		}

		return rez;
	}

	void Explosion::_handleRecursiveRemoval(const Coord& effect_coord) {
		auto [curr_x, curr_y] { effect_coord };

		for (auto [x_offset, y_offset] : Config::DIRECTON_OFFSETS) {
			Coord neighbor_effect_coord{ curr_x + x_offset, curr_y + y_offset };
			Coord neighbor_board_coord{ _mapExplosionToBoard(neighbor_effect_coord) };

			auto [effect_x, effect_y] { neighbor_effect_coord };

			if (m_board.hasStack(neighbor_board_coord) &&
				m_effects[effect_y][effect_x].has_value() && 
				m_visited_effects.contains({effect_x, effect_y}) == false
				) {
				switch (m_effects[effect_y][effect_x].value()) {
				case Effect::HAND:
					_handleBackToHand({ effect_x, effect_y }, neighbor_board_coord);
					break;

				case Effect::REMOVE:
					_handleRemove({ effect_x, effect_y }, neighbor_board_coord);
					break;

				case Effect::HOLE:
					_handleHole({ effect_x, effect_y }, neighbor_board_coord);
					break;

				default:
					break;
				}
			}
		}
	}

	void Explosion::_handleBackToHand(const Coord& effect_coord, const Coord& board_coord) {
		try {
			CombatCard card = std::move(m_board.popTopCardAt(board_coord));

			if (card.getColor() == color::ColorType::RED) {
				m_player1.addCard(std::move(card));
			}
			else {
				m_player2.addCard(std::move(card));
			}

			m_visited_effects.insert(effect_coord);

			_handleRecursiveRemoval(effect_coord);
		}
		catch (const std::runtime_error& e) {
			Logger::log(Level::INFO, "{} coord ({}, {})", e.what(), board_coord.first, board_coord.second);
		}
	}

	void Explosion::_handleRemove(const Coord& effect_coord, const Coord& board_coord) {
		try {
			m_board.removeTopCardAt(board_coord);

			m_visited_effects.insert(effect_coord);

			_handleRecursiveRemoval(effect_coord);
		}
		catch (const std::runtime_error& e) {
			Logger::log(Level::INFO, "{} coord ({}, {})", e.what(), board_coord.first, board_coord.second);
		}
	}

	void Explosion::_handleHole(const Coord& effect_coord, const Coord& board_coord) {
		m_board.createHole(board_coord);
	}

	Coord Explosion::_mapExplosionToBoard(const Coord& effect_coord) {
		Logger::log(Level::INFO, "board {} {} explosion card {} {}", m_board_corner.value().first, m_board_corner.value().second, effect_coord.first, effect_coord.second);

		uint16_t offset_x = Config::getInstance().getCardSpacingX();
		uint16_t offset_y = Config::getInstance().getCardSpacingY();

		return { 
			m_board_corner.value().first  + offset_x * (effect_coord.first - m_effect_corner1.first),
			m_board_corner.value().second + offset_y * (effect_coord.second - m_effect_corner1.second)
		};
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
			if (m_effect_corner2.first + 1 < m_board.size()) {
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
			if (m_effect_corner2.second + 1 < m_board.size()) {
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
		uint16_t begin = (m_board.size() == 3) ? MIN_EFFECTS_3x3 : MIN_EFFECTS_4x4;
		uint16_t end = (m_board.size() == 3) ? MAX_EFFECTS_3x3 : MAX_EFFECTS_4x4;

		std::uniform_int_distribution<uint16_t> random{ begin, end };
		return random(gen);
	}

	std::vector<Coord> Explosion::_generateEffectPos(std::mt19937& gen) {
		auto positions = std::ranges::cartesian_product_view(
			std::views::iota(0, int(m_board.size())),
			std::views::iota(0, int(m_board.size()))
		);
		
		std::vector<Coord> selections;
		
		std::sample(positions.begin(), positions.end(), std::back_inserter(selections), m_effect_count, gen);
		
		return selections;
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

		for (int16_t i = 0; i < m_board.size(); ++i) {
			for (int16_t j = 0; j < m_board.size(); ++j) {
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
		uint16_t size = m_board.size();
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
		uint16_t size = m_board.size();
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


	std::vector<Coord> Explosion::getAffectedFields() const {
		return {};
	}
}