export module ExplosionCard;

import <unordered_map>;
import <vector>;
import <random>;
import <ranges>;

namespace base {

	using Coord = std::pair<int16_t, int16_t>;

	enum class Effect : uint16_t {
		HAND,
		REMOVE,
		HOLE
	};

	export class Explosion {
	public:
		Explosion(uint16_t);

	private:
		uint16_t _generateEffectCount();
		std::vector<Coord>& _generateEffectPos();
		std::vector<Effect>& _generateEffectType();

	private:
		struct CoordFunctor {
			size_t operator()(const Coord& coord) {
				std::hash<uint16_t> hasher;

				return hasher(coord.first) ^ hasher(coord.second << 1);
			}
		};

	private:
		std::random_device m_random;
		std::mt19937 m_generator;

		uint16_t m_effect_count;
		uint16_t m_board_size;
		std::unordered_map<Coord, Effect, CoordFunctor> m_effects;
	};

	
}