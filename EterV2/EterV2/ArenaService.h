#pragma once
#include"Board.h"
#include"Player.h"


namespace base {

    class ArenaService {
    public:
        explicit ArenaService(GameSizeType size, Board& board, Player& player1, Player& player2);

        void addMarker(const Coord& board_coord, color::ColorType color);
        
        std::vector<std::vector<color::ColorType>> getMarkerCoords() const;
        std::optional<std::pair<Coord, color::ColorType>> getWinPos() const;

    private:

    private:
        std::vector<std::vector<color::ColorType>> m_markers;

        std::unordered_map<int16_t, int16_t> m_row_count;
        std::unordered_map<int16_t, int16_t> m_col_count;
        int16_t diag1{ 0 }, diag2{0};

        uint16_t m_size;
        Player& m_player_red;
        Player& m_player_blue;
        Board& m_board;

        std::optional<std::pair<Coord, color::ColorType>> m_win_data;
    };

} 