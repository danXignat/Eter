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
		void setSelectedMove(const Coord& coord, CombatCardType cardType);
		const std::vector<std::pair<Coord, CombatCardType>>& getAvailableChoices() const;


	private:
		bool m_hasUserSelected;
		Coord m_selectedCoord;
		CombatCardType m_selectedCardType;
		std::vector<std::pair<Coord, CombatCardType>> m_availableChoices;

	};

	class Fire :public PowerCard {
	public:
		Fire()=default;

		Fire(Player& red_player, Player& blue_player);

		void apply(Board& board, Player& player) override;
		std::vector<Coord>getDuplicateCards(Board& board, const Player& player);

	private:
		Player& m_red_player;
		Player& m_blue_player;

	};

	class Ash :public PowerCard {
	public:
		Ash();

		void apply(Board& board, Player& player) override;
		void setSelectedMove(const Coord& coord, CombatCardType cardType);

	private:
		bool m_hasUserSelected;
		Coord m_selectedCoord;
		CombatCardType m_selectedCardType;

	};

	class Spark:public PowerCard {
	public:
		Spark();

		void apply(Board& board, Player& player) override;
		std::vector<std::pair<Coord, CombatCardType>> coverCards(const Board& board, const Player& player);
		void setSelectedMove(const Coord& from, const Coord& to, CombatCardType cardType);
		const std::vector<std::pair<Coord, CombatCardType>>& getAvailableChoices() const;

	private:
		bool m_hasUserSelected;
		Coord m_coordFrom;
		Coord m_coordTo;
		CombatCardType m_selectedCardType;
		std::vector<std::pair<Coord, CombatCardType>> m_availableChoices;
	};

	class Squall :public PowerCard {
	public:

		Squall() = default;
		Squall(Player& red_player, Player& blue_player);
		void apply(Board& board, Player& player) override;
		void setSelectedMove(const Coord& coord);

	private:
		bool m_hasUserSelected;
		Coord m_selectedCoord;
		Player& m_red_player, & m_blue_player;
	};

	class Gale :public PowerCard {
	public:
		Gale() = default;
		Gale(Player& red_player, Player& blue_player);

		void apply(Board& board, Player& player) override;


	private:
		Player& m_red_player;
		Player& m_blue_player;
	};

	class Hurricane :public PowerCard {
	public:
		Hurricane();
		
		void apply(Board& board, Player& player) override;
		const std::unordered_map<Orientation, std::vector<uint16_t>>& getOptions(Board& board);
		void setSelectedMove(Orientation orientation, uint16_t index, MoveDirection direction);


	private:
		bool m_hasUserSelected;
		Orientation m_selectedOrientation;
		uint16_t m_selectedIndex;
		MoveDirection m_selectedDirection;
		std::unordered_map<Orientation, std::vector<uint16_t>> m_options;


	};

	class Gust :public PowerCard {
	public:
		Gust();

		void apply(Board& board, Player& player) override;
		std::vector<Coord>getCardsCoord(const Board& board)const;
		void setSelectedMove(const Coord& fromCoord, const Coord& toCoord);
		const std::vector<Coord>& getAvailableCards() const;
		const std::vector<Coord>& getValidMoves(const Coord& selectedCard, Board& board);
		std::vector<Coord> getCardsCoord(const Board& board);


	private:
		bool m_hasUserSelected;
		Coord m_selectedFromCoord;
		Coord m_selectedToCoord;
		std::vector<Coord> m_availableCards;
		std::vector<Coord> m_validMoves;

	};

	class Mirrage :public PowerCard {
	public:
		Mirrage();

		void apply(Board& board, Player& player) override;
		bool getIllusion(Board& board, Player& player);
		void setSelectedMove(const Coord& coord, CombatCardType cardType);

	private:
		bool m_hasUserSelected;
		Coord m_selectedCoord;
		CombatCardType m_selectedCardType;
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
		std::vector<std::pair<Coord, Coord>>getPairs(Board& board);

	};

	class Blizzard :public PowerCard {
	public:
		Blizzard();

		void apply(Board& board, Player& player) override;

	};

	class Waterfall :public PowerCard {
	public:
		Waterfall();
		std::unordered_map < Orientation, std::vector<int16_t>>getOptions(Board& board); 
		std::tuple< Orientation, int16_t, MoveDirection>input(Board&board);
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
		std::vector<std::pair<MoveDirection, std::pair<Coord, Coord>>> checkShifting(
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