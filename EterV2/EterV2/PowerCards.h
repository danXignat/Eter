#pragma once

#include "Board.h"
#include"Player.h"
#include "PowerCard.h"
#include"IllusionService.h"
#include"BaseGameMode.h"

#include<memory>
#include"typedefs.h"
#include "logger.h"
#include<ranges>
#include <algorithm>
#include<map>

namespace base {
	enum class ShiftDirection {
		Right,
		Left,
		Up,
		Down,
		Unknown
	};
	enum class Orientation {
		Row,
		Column,
		Unknown
	};

	class ControllerExplosion :public PowerCard {
	public:
		ControllerExplosion();

		void apply(Board& board, Player& player) override;

	};

	class Destruction :public PowerCard {
	public:
		Destruction();
		void apply( Board& board, Player& player) override;

	};

	class Flame :public PowerCard {
	public:
		Flame();

		void flipIllusion(Board& board, const Player& player);
		void apply(Board& board, Player& player) override;

	};

	class Fire :public PowerCard {
	public:
		Fire();

		void apply(Board& board, Player& player) override;
		std::vector<Coord>getDuplicateCards(Board& board, const Player& player);

	};

	class Ash :public PowerCard {
	public:
		Ash();

		void apply(Board& board, Player& player) override;

	};

	class Spark:public PowerCard {
	public:
		Spark();

		void apply(Board& board, Player& player) override;
		std::vector<std::pair<Coord, CombatCardType>> coverCards(const Board& board, const Player& player);

	};

	class Squall :public PowerCard {
	public:
		Squall();
		std::vector<std::pair<Coord, CombatCard>>opponentCards( Board& board,const Player& player) const;

		void apply(Board& board, Player& player) override;

	};

	class Gale :public PowerCard {
	public:
		Gale();

		void apply(Board& board, Player& player) override;

	};

	class Hurricane :public PowerCard {
	public:
		Hurricane();
		

		void apply(Board& board, Player& player) override;
		std::unordered_map<Orientation, std::vector<uint16_t>> getOptions(Board& board);  
		std::tuple<Orientation, uint16_t, ShiftDirection>  input(Board&board);

	};

	class Gust :public PowerCard {
	public:
		Gust();

		void apply(Board& board, Player& player) override;

	};

	class Mirrage :public PowerCard {
	public:
		Mirrage();

		void apply(Board& board, Player& player) override;
		bool getIllusion(Board& board, Player& player);

	};

	class Storm :public PowerCard {
	public:
		Storm();

		void apply(Board& board, Player& player) override;

	};

	class Tide :public PowerCard {
	public:
		Tide();

		void apply(Board& board, Player& player) override;
		std::vector<Coord>getStacks(const Board& board);

	};

	class Mist :public PowerCard {
	public:
		Mist();

		void apply(Board& board, Player& player) override;
		bool hasIllusion(Board& board, IllusionService& illusionService, Player& player);

	};

	class Wave :public PowerCard {
	public:
		Wave();

		void apply(Board& board, Player& player) override;
		std::vector<Coord>validStacks(const Board& board) const;

	};

	class Whirlpool :public PowerCard {
	public:
		Whirlpool();

		void apply(Board& board, Player& player) override;

	};

	class Blizzard :public PowerCard {
	public:
		Blizzard();

		void apply(Board& board, Player& player) override;

	};

	class Waterfall :public PowerCard {
	public:
		Waterfall();

		void apply(Board& board, Player& player) override;

	};

	class Support :public PowerCard {
	public:
		Support();

		void apply(Board& board, Player& player) override;
		std::vector<Coord>CoordCardType(Board& board, const Player& player) const;

	};

	class Earthquake :public PowerCard {
	public:
		Earthquake();

		void apply(Board& board, Player& player) override;

	};

	class Crumble :public PowerCard {
	public:
		Crumble();

		void apply(Board& board, Player& player) override;
		std::vector<Coord>CoordCardType(Board& board, const Player& player) const;

	};

	class Border :public PowerCard {
	public:
		Border();

		void apply(Board& board, Player& player) override;

	};
	

	class Avalanche :public PowerCard {
	public:
		Avalanche();

		void apply(Board& board, Player& player) override;
		std::vector<std::pair<ShiftDirection, std::pair<Coord, Coord>>> checkShifting(
		const std::vector<std::pair<Orientation, std::pair<Coord, Coord>>>& pack, Board& board);
		std::vector<std::pair<Orientation, std::pair<Coord, Coord>>> getPairs(Board& board);

	};

	class Rock :public PowerCard {
	public:
		Rock();

		void apply(Board& board, Player& player) override;

		std::vector<Coord>getIllusionCoords(const Board& board) const;

	};

}