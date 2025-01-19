#include "ArenaService.h"
#include "Config.h"
#include "utils.h"
#include<algorithm>
#include"logger.h"

using namespace logger;

namespace base {

    ArenaService::ArenaService(GameSizeType size, Player& player1, Player& player2)
        : m_size{ static_cast<uint16_t>((size ==  GameSizeType::SMALL) ? 3 : 4) },
        m_player1{ player1 },
        m_player2{ player2 },
        m_marker{} {
    }

    const std::unordered_map<Coord, color::ColorType, utils::CoordFunctor>ArenaService::getMarker() const {
        return m_marker;
    }

    uint16_t  ArenaService::getSize() const {
        return m_size;
    }

    const Coord& ArenaService::getTransformedCoord() const {
        return m_transformedCoord;
    }

    std::optional<Coord> ArenaService::getWinPosition() const {
        return m_win_pos;
    }

    bool ArenaService::hasWinner() const {
        return m_win_pos.has_value();
    }

    void ArenaService::renderArena(const Board& board) {
        Coord startPos = base::Config::getInstance().getStartPoint();
        startPos.first += 40;

        for (uint16_t y = 0; y < m_size; ++y) {
            for (uint16_t x = 0; x < m_size; ++x) {
                uint16_t displayX = startPos.first + (x * 2);
                uint16_t displayY = startPos.second + y;
                Coord arenaCoord{ x, y };

                auto markerIt = m_marker.find(arenaCoord);
                if (markerIt != m_marker.end()) {
                    std::string marker = "M";
                    std::string coloredMarker = color::to_string(markerIt->second) + marker +
                        color::to_string(color::ColorType::DEFAULT);
                    utils::printAtCoordinate(coloredMarker, displayX, displayY);
                }
                else {
                    utils::printAtCoordinate("*", displayX, displayY);
                }
            }
        }
    }

    Coord ArenaService::getArenaBoard(const Coord& coord, const Board& board) {
        auto [corner1, corner2] = board.getBoudingRect();

        uint16_t relX = (coord.first - corner1.first) / 2;
        uint16_t relY = coord.second - corner1.second;

        uint16_t boardWidth = ((corner2.first - corner1.first) / 2) + 1;
        uint16_t boardHeight = (corner2.second - corner1.second) + 1;

        uint16_t arenaY = static_cast<uint16_t>((static_cast<double>(relY) / (boardHeight - 1)) * (m_size - 1));
        uint16_t arenaX = static_cast<uint16_t>((static_cast<double>(relX) / (boardWidth - 1)) * (m_size - 1));

        return { arenaX, arenaY };
    }

    void ArenaService::placeMarker(const Coord& coord, const Board& board, color::ColorType winnerColor) {

        Coord arenaCoord = getArenaBoard(coord, board);

        auto existingMarker = m_marker.find(arenaCoord);
        if (existingMarker != m_marker.end()) {
            if (existingMarker->second != winnerColor) {
                existingMarker->second = winnerColor;
                Logger::log(Level::INFO, "Replaced opponent's marker at position {}, {}",
                    arenaCoord.first, arenaCoord.second);
            }
            else {
                Logger::log(Level::INFO, "Marker of same color already exists at position {}, {}",
                    arenaCoord.first, arenaCoord.second);
            }
        }
        else {
            m_marker[arenaCoord] = winnerColor;
            Logger::log(Level::INFO, "Placed new marker at position {}, {}",
                arenaCoord.first, arenaCoord.second);
        }
        _checkWinPosition(arenaCoord);
        renderArena(board);
    }


    void ArenaService::_checkWinPosition(const Coord& coord) {
        std::unordered_map<uint16_t, uint16_t>x_count, y_count;
        x_count.reserve(m_size);
        y_count.reserve(m_size);
        int16_t diag1{ 0 }, diag2{ 0 };

        for (const auto& [markerCoord, markerColor] : m_marker) {
            auto [x, y] {markerCoord};
            int16_t incr{ (markerColor == color::ColorType::RED) ? 1 : -1 };
            x_count[x] += incr;
            y_count[y] += incr;

            if (x == y) {
                diag1 += incr;
            }
            if (x + y == m_size - 1) {
                diag2 += incr;
            }
        }

        const auto& [x, y] = coord;
        const uint16_t WIN_COUNT = 3;

        bool won_on_lines = abs(x_count[x] >= WIN_COUNT || abs(y_count[y]) >= WIN_COUNT);
        bool won_on_diags = abs(diag1) >= WIN_COUNT || abs(diag2) >= WIN_COUNT;

        if (won_on_lines || won_on_diags) {
            m_win_pos.emplace(coord);
            Logger::log(Level::INFO, "Win detected in arena at position ({}, {})", x, y);
        }
    }


    uint16_t ArenaService::countMarkers(color::ColorType color) const {
        return std::count_if(m_marker.begin(), m_marker.end(),
            [color](const auto& pair) { return pair.second == color; });
    }
}