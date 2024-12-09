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
		bool validPosition(const Coord& coord, Board& board, const Player& player)const;
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
		bool areAdjacentCards(const Coord& coord, Board& board);
		void apply(Board& board, Player& player) override;
		std::vector<Coord> getChoices(Board& board, Player& player);
		bool allCardsHaveNeighbors( Board&board);
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
		void apply(Board& board, Player& player) override;

	};

}

