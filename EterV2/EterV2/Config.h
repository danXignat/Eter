#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include <variant>
#include <array>
#include <ranges>
#include <algorithm>
#include "typedefs.h"

namespace base {
    class Config {
    public:

        static Config& getInstance();

        std::unordered_map<std::string, std::variant<uint16_t, Coord>> getConfig() const;
        Config& setStartPoint(const Coord&);
        Config& setCardSpacingX(uint16_t);
        Config& setCardSpacingY(uint16_t);
        Config& setFetchByID(bool);

        Coord    getStartPoint() const;
        Coord    getSpacing() const;
        uint16_t getCardSpacingX() const;
        uint16_t getCardSpacingY() const;
        bool getFetchByID() const;

        const std::array<Coord, 8>& getOffsets() const;
    
    public:
        Config(const Config&) = delete;
        Config& operator=(const Config&) = delete;
        ~Config() = default;

    public:
        static constexpr const std::array<Coord, 8> DIRECTON_OFFSETS{
            Coord{1, 0}, Coord{1, -1}, Coord{0, -1}, Coord{-1, -1},
            Coord{-1, 0}, Coord{-1, 1}, Coord{0, 1}, Coord{1, 1}
        };

    private:
        Config();

    private:
        static std::unique_ptr<Config> m_instance;

        std::array<Coord, 8> m_mapped_offsets;
        Coord    m_start_point;
        uint16_t m_card_space_x;
        uint16_t m_card_space_y;
        bool m_fetch_by_ID;
    };
}