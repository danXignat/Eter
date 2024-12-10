#pragma once

#include "Board.h"
#include"Player.h"
#include "PowerCard.h"

#include<memory>
#include"typedefs.h"
#include "logger.h"

namespace base {

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
		std::vector<std::pair<Coord, CombatCard>>opponentCards( Board& board, Player& opponent);

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

	};

	class Wave :public PowerCard {
	public:
		Wave();

		void apply(Board& board, Player& player) override;

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

	};

	class Rock :public PowerCard {
	public:
		Rock();

		void apply(Board& board, Player& player) override;

	};

}