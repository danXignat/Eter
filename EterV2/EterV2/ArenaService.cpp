#include "ArenaService.h"
#include "Config.h"
#include "utils.h"
#include<algorithm>
#include"logger.h"

using namespace logger;

namespace base {

    ArenaService::ArenaService(GameSizeType size, Player& player1, Player& player2)
        : m_size{ static_cast<uint16_t>(size) },
        m_player1{player1},
        m_player2{player2},
        m_marker{} {
    }

    ArenaService::ArenaService(GameSizeType size, const Coord& startCoord, Player& player1, Player& player2)
        : m_size{ static_cast<uint16_t>(size) }, 
        m_player1{ player1 },
        m_player2{ player2 }, 
        m_marker{} {
    }

    void ArenaService::renderArena(const Board& board) {

        Coord startPos = base::Config::getInstance().getStartPoint();
        startPos.first += 40; 

        uint16_t arenaWidth = m_size;
        uint16_t arenaHeight = m_size;

        for (uint16_t y = 0; y < arenaHeight; ++y) {
            for (uint16_t x = 0; x < arenaWidth; ++x) {
                Coord pos = { startPos.first + x * 2, startPos.second + y };
                Coord arenaCoord = { x, y };
                auto markerIt = m_marker.find(arenaCoord);
                if (m_marker.find(arenaCoord) != m_marker.end()) {

                    std::string marker = "M";
                    std::string coloredMarker = color::to_string(markerIt->second) + marker + color::to_string(color::ColorType::BLUE);
                    utils::printAtCoordinate(coloredMarker, pos.first, pos.second);
                }

                    else if (x == 0 || x == arenaWidth - 1 || y == 0 || y == arenaHeight - 1) {
                        utils::printAtCoordinate("*", pos.first, pos.second);
                    }
                    else {
                        utils::printAtCoordinate("*", pos.first, pos.second);
                    }
            }
        }
    }

    Coord ArenaService::getArenaBoard(const Coord& coord, const Board& board) {
        uint16_t mainBoardSize = board.getSize();
        uint16_t arenaSize = m_size;

        double scaleX = arenaSize / (board.getBoundingRectSize().first / 2);
        double scaleY = arenaSize / board.getBoundingRectSize().second;

        uint16_t arenaX = coord.first * scaleX;
        uint16_t arenaY = coord.second * scaleY;

        arenaX = std::min<uint16_t>(arenaX, arenaSize - 1);
        arenaY = std::min<uint16_t>(arenaY, arenaSize - 1);

      return  m_transformedCoord = { arenaX, arenaY };
    }

    const Coord& ArenaService::getTransformedCoord() const {
        return m_transformedCoord;
    }

    uint16_t ArenaService::getSize() const {
        return m_size;
    }

    const std::unordered_map<Coord, color::ColorType, utils::CoordFunctor>ArenaService:: getMarker() {
        return m_marker;
    }

    void ArenaService::placeMarker(const Coord& coord, const Board& board, color::ColorType winnerColor) {
        std::optional<Coord> winCoord = board.getWinCoord();
        if (!winCoord.has_value()) {
            Logger::log(Level::ERROR, "No winning coordinate available");
            return;
        }

        Coord arenaCoord = getArenaBoard(winCoord.value(), board);
        auto existingMarker = m_marker.find(arenaCoord);

        if (existingMarker != m_marker.end()) {
            if (existingMarker->second != winnerColor) {
                existingMarker->second = winnerColor;
                Logger::log(Level::INFO, "Replaced opponent's marker at position", arenaCoord.first, ",", arenaCoord.second);
            }
            else {
                Logger::log(Level::INFO, "Marker of same color already exists at position", arenaCoord.first, ",", arenaCoord.second);
            }
        }
        else {
            m_marker[arenaCoord] = winnerColor;
            Logger::log(Level::INFO, "Placed new marker at position", arenaCoord.first, ",", arenaCoord.second);
        }

        renderArena(board);
    }


} 