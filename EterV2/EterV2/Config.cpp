#include "Config.h"

namespace base {
    std::unique_ptr<Config> Config::m_instance = nullptr;

	Config& Config::getInstance() {
        if (!m_instance) {
            m_instance = std::unique_ptr<Config>(
                new Config()
            );
        }
        return *m_instance;
    }

    Config& Config::setStartPoint(const Coord& coord) {
        m_instance->m_start_point = coord;

        return *m_instance;
    }

    Config& Config::setCardWidth(uint16_t width) {
        m_instance->m_card_width = width;

        return *m_instance;
    }

    Config& Config::setCardHeight(uint16_t height) {
        m_instance->m_card_height = height;

        return *m_instance;
    }

    Coord Config::getStartPoint(const Coord&) {
        return m_start_point;
    }

    uint16_t Config::getCardWidth(uint16_t) {
        return m_card_width;
    }

    uint16_t Config::getCardHeight(uint16_t) {
        return m_card_height;
    }

    std::unordered_map<std::string, std::variant<uint16_t, Coord>> Config::getConfig() const {
        return {
            {"StartPoint", {m_start_point}},
            {"CardWidth", {m_card_width} },
            {"CardHeight", {m_card_height} }
        };
    }
}