#include "ArenaService.h"
#include "Config.h"
#include "utils.h"
#include<algorithm>
#include"logger.h"

using namespace logger;

namespace base {

    ArenaService::ArenaService(GameSizeType size, Board& board, Player& player1, Player& player2)
        : m_size{ static_cast<uint16_t>((size ==  GameSizeType::SMALL) ? 3 : 4) },
        m_player_red{ player1 },
        m_player_blue{ player2 },
        m_board{board},
        m_markers{ m_size, std::vector<color::ColorType>(m_size, color::ColorType::DEFAULT) } {

    }
    
    void ArenaService::addMarker(const Coord& board_coord, color::ColorType color) {
        Coord mapped{ utils::mapCoordToMatrix(m_board.getLeftCorner(), board_coord) };
        auto [row, col] {mapped};

        m_markers[row][col] = color;

        int16_t increment{ (color == color::ColorType::RED) ? -1 : 1 };

        m_row_count[row]+=increment;
        m_col_count[col]+=increment;

        if (row == col) {
            diag1+=increment;
        }

        if (row + col == m_size - 1) {
            diag2+=increment;
        }

        if (abs(m_row_count[row]) == m_size || abs(m_col_count[col]) == m_size
            || abs(diag1) == m_size || abs(diag2) == m_size) {
            m_win_data.emplace(mapped, color);
        }
    }

    std::vector<std::vector<color::ColorType>> ArenaService::getMarkerCoords() const {
        return m_markers;
    }

    std::optional<std::pair<Coord, color::ColorType>> ArenaService::getWinPos() const {
        return m_win_data;
    }
}