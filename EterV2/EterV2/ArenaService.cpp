#include "ArenaService.h"
#include "Config.h"
#include "utils.h"
#include<algorithm>
#include"logger.h"

using namespace logger;

namespace base {

    ArenaService::ArenaService(GameSizeType size, Player& player1, Player& player2)
        : m_size{ static_cast<uint16_t>(size) },
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

    void ArenaService::renderArena(const Board& board) {
        Coord startPos = base::Config::getInstance().getStartPoint();
        startPos.first += 40;

        for (uint16_t y = 0; y < m_size; ++y) {
            for (uint16_t x = 0; x < m_size; ++x) {
                uint16_t displayX = startPos.first + (x * 2);
                uint16_t displayY = startPos.second + y;
                Coord arenaCoord{ x, y };

                if (m_marker.find(arenaCoord) != m_marker.end()) {
                    std::string marker = "M";
                    auto& markerColor = m_marker[arenaCoord];
                    std::string coloredMarker = color::to_string(markerColor) + marker +
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

        renderArena(board);
    }

    void ArenaService::checkAndPlaceWinMarker(const Board& board) {
        auto winCoord = board.getWinCoord();
        if (winCoord.has_value()) {
            const auto& stack = board[winCoord.value()];
            if (!stack.empty()) {
                color::ColorType winnerColor = stack.back().getColor();
                placeMarker(winCoord.value(), board, winnerColor);
                Logger::log(Level::INFO, "Win detected at ({}, {}), placing marker for {}",
                    winCoord.value().first, winCoord.value().second,
                    (winnerColor == color::ColorType::RED ? "RED" : "BLUE"));
            }
        }
    }

    uint16_t ArenaService::countMarkers(color::ColorType color) const {
        return std::count_if(m_marker.begin(), m_marker.end(),
            [color](const auto& pair) { return pair.second == color; });
    }
}