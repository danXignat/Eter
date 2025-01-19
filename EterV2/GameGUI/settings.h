#pragma once
#include <cstdint>
#include <qpoint.h>
#include <math.h>

constexpr const int CARD_SIZE{ 80 };
constexpr const int CARD_OFFSET{ 10 };

constexpr const int EXPLOSION_SIZE{ 160 };
constexpr const int EFFECT_SIZE{ 40 };
constexpr const int VORTEX_SIZE{ 250 };
const QPointF VORTEX_POS{ 1000, 200 };
constexpr const int VERTEX_RANGE{ 100 };

constexpr const int WINDOW_WIDTH{ 1200 };
constexpr const int WINDOW_HEIGHT{ 800 };

constexpr const char* const CARD_PATH{ "../pictures/cards/%1_%2.png" };
constexpr const char* const BACK_PATH{ "../pictures/cards/%1_%2.png" };
constexpr const char* const EXPLOSION_3x3_PATH{ "../pictures/explosion/3x3.png" };
constexpr const char* const EXPLOSION_4x4_PATH{ "../pictures/explosion/4x4.png" };
constexpr const char* const HAND_PATH{ "../pictures/explosion/hand.png" };
constexpr const char* const REMOVE_PATH{ "../pictures/explosion/remove.png" };
constexpr const char* const HOLE_PATH{ "../pictures/explosion/hole.png" };
constexpr const char* const VORTEX_PATH{ "../pictures/explosion/vortex2.png" };

constexpr const char* const ICON_PATH{ "../pictures/icon.png" };