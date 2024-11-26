#pragma once
#include <random>

#include "typedefs.h"

namespace base {

	enum class Effect : uint16_t {
		HAND,
		REMOVE,
		HOLE
	};

	class Explosion {
	public:
		Explosion(uint16_t);

		void render();
		void rotateLeft();
		void rotateRight();

	public:
		static constexpr uint16_t MIN_EFFECTS_3x3 = 2;
		static constexpr uint16_t MAX_EFFECTS_3x3 = 4;
		static constexpr uint16_t MIN_EFFECTS_4x4 = 3;
		static constexpr uint16_t MAX_EFFECTS_4x4 = 6;

	private:
		uint16_t _generateEffectCount(std::mt19937&);
		std::vector<Coord> _generateEffectPos (std::mt19937&);
		std::vector<Effect> _generateEffectType(std::mt19937&);

	private:
		const uint16_t m_board_size;
		uint16_t m_effect_count;
		std::vector<std::vector<std::optional<Effect>>> m_effects;
	};


}