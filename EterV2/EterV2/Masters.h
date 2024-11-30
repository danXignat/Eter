#pragma once

#include "Board.h"
#include "Player.h"
#include "MageCard.h"
#include "typedefs.h"
#include "logger.h"

namespace base {
	class MasterOfFireFront : public MageCard {
	public:
		MasterOfFireFront();

		void apply(Board& board, Player& player) override;

		std::vector<Coord> getChoices(const Board& board, const Player& player);

	};

	class MasterOfFireBack : public MageCard {
	public:
		MasterOfFireBack();

		void apply(Board& board, Player& player) override;

		std::pair<std::vector<uint16_t>, std::vector<uint16_t>> getChoices(const Board& board, const Player& player);

	};

	class MasterOfEarthFront : public MageCard {
	public:
		MasterOfEarthFront();

		void apply(Board& board, Player& player) override;
		std::vector<Coord> getChoices(Board& board, const Player& player);
	};

	class MasterOfEarthBack : public MageCard {
	public:
		MasterOfEarthBack();

		void apply(Board& board, Player& player) override;
		
	};

	class MasterOfAirFront : public MageCard {
	public:
		MasterOfAirFront();

		void apply(Board& board, Player& player) override;
		std::vector<Coord> getChoices(Board& board, const Player& player);

	};

	class MasterOfAirBack : public MageCard {
	public:
		MasterOfAirBack();

		void apply(Board& board, Player& player) override;
	};

	class MasterOfWaterFront : public MageCard {
	public:
		MasterOfWaterFront();

		void apply(Board& board, Player& player) override;
		std::vector<Coord> getChoices(Board& board, Player& player);
	};

	class MasterOfWaterBack : public MageCard {
	public:
		MasterOfWaterBack();

		void apply(Board& board, Player& player) override;
	};

}