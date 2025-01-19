#pragma once
#include "qt_includes.h"
#include "CardTypes.h"
#include "..\EterV2\GameModeFactory.h"

namespace gui {
    namespace utils {
        inline QPointF coordToQPointF(const base::Coord& coord) {
            return QPointF(static_cast<qreal>(coord.first), static_cast<qreal>(coord.second));
        }

        inline base::Coord qPointFToCoord(const QPointF& point) {
            return base::Coord{ static_cast<int16_t>(point.x()), static_cast<int>(point.y()) };
        }    
    }
}

namespace std {
    template<>
    struct hash<QPointF> {
        size_t operator()(const QPointF& point) const {
            return qHashMulti(0, point.x(), point.y());
        }
    };
}