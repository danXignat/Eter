#pragma once
#include <cstdint>
#include <qpoint.h>
#include <math.h>

constexpr const int CARD_SIZE		{ 80 };
constexpr const int RED_X_PADDING	{ 10 };
constexpr const int CARD_OFFSET		{ 10 };

constexpr const int SPECIAL_CARD_SIZE	{ 145 };
constexpr const int EFFECT_SIZE			{ 37 };
constexpr const int INFO_ICON_SIZE		{ 37 };
constexpr const int VORTEX_SIZE			{ 250 };
constexpr const int ARENA_SIZE			{ 225 };
constexpr const int MARKER_SIZE			{ 40 };
constexpr const int VERTEX_RANGE		{ 100 };

const QPointF VORTEX_POS		{ 1000, 200 };
const QPointF EXPLOSION_POS		{ 1119, 450 };
const QPointF MAGE_POS			{ 1148, 713 };
const QPointF POWER_POS1		{ 167,341 };
const QPointF POWER_POS2		{ 167,557 };
const QPointF ARENA_POS			{ 160, 125 };

constexpr const int WINDOW_WIDTH	{ 1300 };
constexpr const int WINDOW_HEIGHT	{ 900 };

constexpr const int VICTORY_IMAGE_WIDTH = 300;
constexpr const int VICTORY_IMAGE_HEIGHT = 300;
constexpr const int BUTTON_WIDTH = 300;
constexpr const int BUTTON_HEIGHT = 300;

constexpr const char* const MAGE_PATH			{ "../pictures/mages/%1" };
constexpr const char* const CURSOR_PATH			{ "../pictures/estetics/custom_cursor.png" };
constexpr const char* const CARD_PATH			{ "../pictures/cards/%1_%2.png" };
constexpr const char* const BACK_PATH			{ "../pictures/cards/%1_%2.png" };
constexpr const char* const EXPLOSION_3x3_PATH	{ "../pictures/explosion/3x3.png" };
constexpr const char* const EXPLOSION_4x4_PATH	{ "../pictures/explosion/4x4.png" };
constexpr const char* const HAND_PATH			{ "../pictures/explosion/hand.png" };
constexpr const char* const REMOVE_PATH			{ "../pictures/explosion/remove.png" };
constexpr const char* const HOLE_PATH			{ "../pictures/explosion/hole.png" };
constexpr const char* const VORTEX_PATH			{ "../pictures/explosion/vortex2.png" };
constexpr const char* const ICON_PATH			{ "../pictures/icon.png" };
