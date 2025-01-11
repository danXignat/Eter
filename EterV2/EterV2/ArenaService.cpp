#include "ArenaService.h"
#include "Config.h"
#include "utils.h"
#include<algorithm>

namespace base {

    ArenaService::ArenaService(GameSizeType size, Player& player1, Player& player2)
        : m_size{ static_cast<uint16_t>(size) },
        m_player1{player1},
        m_player2{player2}{
    }

    ArenaService::ArenaService(GameSizeType size, const Coord& startCoord, Player& player1, Player& player2)
        : m_size{ static_cast<uint16_t>(size) }, 
        m_player1{ player1 },
        m_player2{ player2 } {
    }

    void ArenaService::renderArena(const Board& board) {

        Coord startPos = base::Config::getInstance().getStartPoint();
        startPos.first += 40; 

        uint16_t arenaWidth = m_size;
        uint16_t arenaHeight = m_size;

        for (uint16_t y = 0; y < arenaHeight; ++y) {
            for (uint16_t x = 0; x < arenaWidth; ++x) {
                Coord pos = { startPos.first + x * 2, startPos.second + y };

                if (x == 0 || x == arenaWidth - 1 || y == 0 || y == arenaHeight - 1) {
                    utils::printAtCoordinate("*", pos.first, pos.second);
                }
                else {
                    utils::printAtCoordinate("*", pos.first, pos.second);
                }
            }
        }
    }

    void ArenaService::getArenaBoard(const Coord& coord, const Board& board) {
        uint16_t mainBoardSize = board.getSize();
        uint16_t arenaSize = m_size;

        double scaleX = arenaSize / (board.getBoundingRectSize().first / 2);
        double scaleY = arenaSize / board.getBoundingRectSize().second;

        uint16_t arenaX = coord.first * scaleX;
        uint16_t arenaY = coord.second * scaleY;

        arenaX = std::min<uint16_t>(arenaX, arenaSize - 1);
        arenaY = std::min<uint16_t>(arenaY, arenaSize - 1);

        m_transformedCoord = { arenaX, arenaY };
    }

    const Coord& ArenaService::getTransformedCoord() const {
        return m_transformedCoord;
    }

    uint16_t ArenaService::getSize() const {
        return m_size;
    }

} 