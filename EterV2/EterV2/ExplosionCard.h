#pragma once
#include <random>
#include <unordered_map>

#include "Board.h"
#include "typedefs.h"

namespace base {

	enum class Effect : uint16_t {
		HAND,
		REMOVE,
		HOLE
	};

	enum class Direction : uint16_t {
		RIGHT,
		LEFT,
		UP, 
		DOWN,
		NONE
	};

	class Explosion {
	public:
		bool color = false;

		Explosion(Board&, Player&, Player&);

		std::vector<Coord> getAffectedFields() const;

		void apply();
		void render();
		void setEffectForWidth(Board&);
		void setEffectForHeight(Board&);
		void moveEffect(Direction);
		
		void rotateLeft();
		void rotateRight();

	public:
		static constexpr const uint16_t MIN_EFFECTS_3x3 = 2;
		static constexpr const uint16_t MAX_EFFECTS_3x3 = 4;
		static constexpr const uint16_t MIN_EFFECTS_4x4 = 3;
		static constexpr const uint16_t MAX_EFFECTS_4x4 = 6;

	private:
		uint16_t _generateEffectCount(std::mt19937&);
		std::vector<Coord> _generateEffectPos (std::mt19937&);
		std::vector<Effect> _generateEffectType(std::mt19937&);

		void _handleBackToHand(const Coord&, const Coord&);
		void _handleRemove(const Coord&, const Coord&);
		void _handleHole(const Coord&, const Coord&);
		void _handleRecursiveRemoval(const Coord&);
		Coord _mapExplosionToBoard(const Coord&);
		Coord _mapBoardToExplosion(const Coord&);

	private:
		Board& m_board;
		Player& m_player1, & m_player2;

		uint16_t m_effect_count;
		std::optional<Coord> m_board_corner;
		std::unordered_set<Coord, utils::CoordFunctor> m_visited_effects;
		std::vector<std::vector<std::optional<Effect>>> m_effects;
		Coord m_effect_corner1, m_effect_corner2;
	};


}