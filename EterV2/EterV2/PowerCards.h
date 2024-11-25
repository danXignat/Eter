#pragma once

#include "Board.h"
#include"Player.h"
#include "PowerCard.h"

#include<memory>
#include"typedefs.h"

namespace base {

	class ControllerExplosion :public PowerCard {
	public:
		ControllerExplosion();

		void apply() override;

	};

	class Destruction :public PowerCard {
	public:
		Destruction();

		void apply() override;

	};

	class Flame :public PowerCard {
	public:
		Flame();

		void apply() override;

	};

	class Fire :public PowerCard {
	public:
		Fire();

		void apply() override;

	};

	class Ash :public PowerCard {
	public:
		Ash();

		void apply() override;

	};

	class Spark:public PowerCard {
	public:
		Spark();

		void apply() override;

	};

	class Squall :public PowerCard {
	public:
		Squall();

		void apply() override;

	};

	class Gale :public PowerCard {
	public:
		Gale();

		void apply() override;

	};

	class Hurricane :public PowerCard {
	public:
		Hurricane();

		void apply() override;

	};

	class Gust :public PowerCard {
	public:
		Gust();

		void apply() override;

	};

	class Mirrage :public PowerCard {
	public:
		Mirrage();

		void apply() override;

	};

	class Storm :public PowerCard {
	public:
		Storm();

		void apply() override;

	};

	class Tide :public PowerCard {
	public:
		Tide();

		void apply() override;

	};

	class Mist :public PowerCard {
	public:
		Mist();

		void apply() override;

	};

	class Wave :public PowerCard {
	public:
		Wave();

		void apply() override;

	};

	class Whirlpool :public PowerCard {
	public:
		Whirlpool();

		void apply() override;

	};

	class Blizzard :public PowerCard {
	public:
		Blizzard();

		void apply() override;

	};

	class Waterfall :public PowerCard {
	public:
		Waterfall();

		void apply() override;

	};

	class Support :public PowerCard {
	public:
		Support();

		void apply() override;

	};

	class Earthquake :public PowerCard {
	public:
		Earthquake();

		void apply() override;

	};

	class Crumble :public PowerCard {
	public:
		Crumble();

		void apply() override;

	};

	class Border :public PowerCard {
	public:
		Border();

		void apply() override;

	};

	class Avalanche :public PowerCard {
	public:
		Avalanche();

		void apply() override;

	};

	class Rock :public PowerCard {
	public:
		Rock();

		void apply() override;

	};

}