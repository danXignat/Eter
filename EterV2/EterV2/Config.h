#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include <variant>

#include "typedefs.h"

namespace base {
    class Config {
    public:
        static Config& getInstance();

        std::unordered_map<std::string, std::variant<uint16_t, Coord>> getConfig() const;
        Config& setStartPoint(const Coord&);
        Config& setCardWidth(uint16_t);
        Config& setCardHeight(uint16_t);

        Coord getStartPoint(const Coord&);
        uint16_t getCardWidth(uint16_t);
        uint16_t getCardHeight(uint16_t);

        struct Visitor {
            void operator()(const Coord&);
            void operator()(uint16_t);
        };

        Config(const Config&) = delete;
        Config& operator=(const Config&) = delete;
        ~Config() = default;
    private:
        Config() = default;

    private:
        static std::unique_ptr<Config> m_instance;

        Coord    m_start_point;
        uint16_t m_card_width;
        uint16_t m_card_height;
    };
}