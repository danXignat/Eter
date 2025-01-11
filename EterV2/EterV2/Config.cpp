#include "Config.h"

namespace base {
    std::unique_ptr<Config> Config::m_instance{new Config()};

    Config::Config() :
        m_mapped_offsets{DIRECTON_OFFSETS},
        m_start_point{0, 0},
        m_card_space_x{ 0 },
        m_card_space_y{ 0 } {

    }

	Config& Config::getInstance() {
        return *m_instance;
    }

    Config& Config::setStartPoint(const Coord& coord) {
        m_instance->m_start_point = coord;

        return *m_instance;
    }

    Config& Config::setCardSpacingX(uint16_t spacing) {
        m_instance->m_card_space_x = spacing;

        std::transform(m_mapped_offsets.begin(), m_mapped_offsets.end(), m_mapped_offsets.begin(),
            [spacing](auto& p) -> Coord { return { spacing * p.first, p.second }; }
        );

        return *m_instance;
    }

    Config& Config::setCardSpacingY(uint16_t spacing) {
        m_instance->m_card_space_y = spacing;

        std::transform(m_mapped_offsets.begin(), m_mapped_offsets.end(), m_mapped_offsets.begin(),
            [spacing](auto& p) -> Coord { return {p.first, spacing * p.second}; }
        );

        return *m_instance;
    }

    Coord Config::getStartPoint() const {
        return m_start_point;
    }

    Coord Config::getSpacing() const {
        return { m_card_space_x, m_card_space_y };
    }

    uint16_t Config::getCardSpacingX() const {
        return m_card_space_x;
    }

    uint16_t Config::getCardSpacingY() const {
        return m_card_space_y;
    }

    const std::array<Coord, 8>& Config::getOffsets() const {
        return m_mapped_offsets;
    }

    std::unordered_map<std::string, std::variant<uint16_t, Coord>> Config::getConfig() const {
        return {
            {"StartPoint", {m_start_point}},
            {"CardSpaceX", {m_card_space_x} },
            {"CardSpaceY", {m_card_space_y} }
        };
    }
}