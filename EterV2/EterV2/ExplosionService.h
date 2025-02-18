#pragma once
#include <random>

#include "Board.h"
#include "Player.h"
#include "logger.h"

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

	class ExplosionService {
	public:
		bool color = false;

	public:
		static constexpr const uint16_t MIN_EFFECTS_3x3 = 2;
		static constexpr const uint16_t MAX_EFFECTS_3x3 = 4;
		static constexpr const uint16_t MIN_EFFECTS_4x4 = 3;
		static constexpr const uint16_t MAX_EFFECTS_4x4 = 6;

	public:
		static constexpr const uint16_t VALID_COMPLETED_LINES = 2;

		ExplosionService(Board&, Player&, Player&);

		void apply();
		void setEffectForWidth(Board&);
		void setEffectForHeight(Board&);
		void moveEffect(Direction);

		void rotateLeft();
		void rotateRight();

		std::vector<Coord> getAffectedFields() const;
		std::unordered_map<Coord, Effect, utils::CoordFunctor> getEffectCoords() const;

		void generate();
		void setting();
		void render();
		bool checkAvailability() const;
		bool used() const;
		uint16_t size() const;

	private:
		uint16_t _generateEffectCount(std::mt19937&);
		std::vector<Coord> _generateEffectPos(std::mt19937&);
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
		bool m_used{false};
	};
}