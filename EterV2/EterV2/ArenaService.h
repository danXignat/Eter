#pragma once
#include"Board.h"
#include"Player.h"


namespace base {

    class ArenaService {
    public:
        explicit ArenaService(GameSizeType size, Player& player1, Player& player2);

        void renderArena(const Board& board);
        Coord getArenaBoard(const Coord& coord, const Board& board);

        // Getters
        const Coord& getTransformedCoord() const;
        uint16_t getSize() const;
        const std::unordered_map<Coord, color::ColorType, utils::CoordFunctor> getMarker() const ;
        std::optional<Coord>getWinPosition() const;

        void placeMarker(const Coord& coord, const Board& board, color::ColorType winnerColor);
        uint16_t countMarkers(color::ColorType color) const;
        bool hasWinner() const;


    private:
        void _checkWinPosition(const Coord& coord);

    private:
        std::unordered_map<Coord, color::ColorType, utils::CoordFunctor>m_marker;
        uint16_t m_size;
        Coord m_transformedCoord;
        Player& m_player1;
        Player& m_player2;

        std::optional<Coord>m_win_pos;
    };

} 