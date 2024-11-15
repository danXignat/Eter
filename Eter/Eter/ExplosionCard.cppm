export module ExplosionCard;

import <unordered_map>;
import <vector>;
import <random>;
import <ranges>;
import <optional>;

import utils;

namespace base {
	using utils::Coord;

	enum class Effect : uint16_t {
		HAND,
		REMOVE,
		HOLE
	};

	export class Explosion {
	public:
		Explosion(uint16_t);

		void render();
		void rotateLeft();
		void rotateRight();

	private:
		uint16_t _generateEffectCount();
		std::vector<Coord> _generateEffectPos();
		std::vector<Effect> _generateEffectType();

	private:
		std::random_device m_random;
		std::mt19937 m_generator;

		const uint16_t m_board_size;
		uint16_t m_effect_count;
		std::vector<std::vector<std::optional<Effect>>> m_effects;
	};

	
}