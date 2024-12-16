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

		bool apply(Board& board, Player& player) override;

		std::vector<Coord> getChoices(const Board& board, const Player& player);

	};

	class MasterOfFireBack : public MageCard {
	public:
		MasterOfFireBack();

		bool apply(Board& board, Player& player) override;

		std::pair<std::vector<uint16_t>, std::vector<uint16_t>> getChoices(const Board& board, const Player& player);

	};

	class MasterOfEarthFront : public MageCard {
	public:
		MasterOfEarthFront();

		bool apply(Board& board, Player& player) override;
		bool validPosition(const Coord& coord, Board& board, const Player& player)const;
		std::vector<Coord> getChoices(Board& board, const Player& player);
	};

	class MasterOfEarthBack : public MageCard {
	public:
		MasterOfEarthBack();

		bool apply(Board& board, Player& player) override;

	};

	class MasterOfAirFront : public MageCard {
	public:
		MasterOfAirFront();

		bool apply(Board& board, Player& player) override;
		std::vector<Coord> getChoices(Board& board, const Player& player);

	};

	class MasterOfAirBack : public MageCard {
	public:
		MasterOfAirBack();

		bool apply(Board& board, Player& player) override;
	};

	class MasterOfWaterFront : public MageCard {
	public:
		MasterOfWaterFront();
		bool apply(Board& board, Player& player) override;
		std::vector<Coord> getChoices(Board& board, Player& player);
	};
	enum class BorderType {
		Top,
		Bottom,
		Left,
		Right
	};
	class MasterOfWaterBack : public MageCard {
	public:
		MasterOfWaterBack();
		std::optional<std::unordered_map<BorderType, std::vector<Coord>>> getBorders(Board& board, Player& player) const;
		std::pair<uint16_t,char> selectBorders(const std::optional<std::unordered_map<BorderType, std::vector<Coord>>>& borders,Board&board)const ;
	 bool apply(Board& board, Player& player) override;

	};

}

