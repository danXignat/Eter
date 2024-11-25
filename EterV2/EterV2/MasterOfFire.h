#pragma once

#include "Board.h"
#include "Player.h"
#include "MageCard.h"

#include "typedefs.h"

namespace base {
	class MasterOfFireFront : public MageCard {
	public:
		MasterOfFireFront();

		void apply(const Coord& coord, Board&board, const Player& player) override;
	};

	class MasterOfFireBack : public MageCard {
	public:
		MasterOfFireBack();

		void apply(const Coord& coord, Board& board, const Player& player) override;
	};

	class MasterOfEarthFront : public MageCard {
	public:
		MasterOfEarthFront();

		void apply(const Coord& coord, Board& board, const Player& player) override;
	};

	class MasterOfEarthBack : public MageCard {
	public:
		MasterOfEarthBack();

		void apply(const Coord& coord, Board& board, const Player& player) override;
	};

	class MasterOfAirFront : public MageCard {
	public:
		MasterOfAirFront();

		void apply(const Coord& coord, Board& board, const Player& player) override;
	};

	class MasterOfAirBack : public MageCard {
	public:
		MasterOfAirBack();

		void apply(const Coord& coord, Board& board, const Player& player) override;
	};

	class MasterOfWaterFront : public MageCard {
	public:
		MasterOfWaterFront();

		void apply(const Coord& coord, Board& board, const Player& player) override;
	};

	class MasterOfWaterBack : public MageCard {
	public:
		MasterOfWaterBack();

		void apply(const Coord& coord, Board& board, const Player& player) override;
	};

}